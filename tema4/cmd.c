#include "cmd.h"

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>

#include "utils.h"

static bool shell_cd(word_t *dir) {
	if (dir == NULL || dir->string == NULL || chdir(dir->string))
		return false;

	return true;
}

static int shell_exit(void) {
	exit(SHELL_EXIT);
}

static void shell_redirect_stdin(simple_command_t *s) {
	char path[1024];

	snprintf(path, sizeof(path), "%s", s->in->string);

	if (s->in->next_part)
		strcat(path, get_word(s->in->next_part));

	int fd = open(path, O_RDONLY, 0644);

	dup2(fd, STDIN_FILENO);
	close(fd);
}

static void shell_redirect_stdout(simple_command_t *s) {
	char path[1024];

	snprintf(path, sizeof(path), "%s", s->out->string);

	if (s->out->next_part)
		strcat(path, get_word(s->out->next_part));

	int flags = O_WRONLY | O_CREAT;

	if (s->err || s->io_flags == IO_OUT_APPEND)
		flags = flags | O_APPEND;
	else
		flags = flags | O_TRUNC;

	int fd = open(path, flags, 0644);

	dup2(fd, STDOUT_FILENO);
	close(fd);
}

static void shell_redirect_stderr(simple_command_t *s) {
	char path[1024];

	snprintf(path, sizeof(path), "%s", s->err->string);

	if (s->err->next_part)
		strcat(path, get_word(s->err->next_part));

	int flags = O_WRONLY | O_CREAT;

	if (s->out || s->io_flags == IO_REGULAR)
		flags = flags | O_TRUNC;
	else
		flags = flags | O_APPEND;

	int fd = open(path, flags, 0644);

	dup2(fd, STDERR_FILENO);
	close(fd);
}

static void shell_redirect(simple_command_t *s) {
	int stdin_fd = dup(STDIN_FILENO);
	int stdout_fd = dup(STDOUT_FILENO);
	int stderr_fd = dup(STDERR_FILENO);

	s->in ? shell_redirect_stdin(s) : NULL;
	s->out ? shell_redirect_stdout(s) : NULL;
	s->err ? shell_redirect_stderr(s) : NULL;

	dup2(stdin_fd, STDIN_FILENO);
	dup2(stdout_fd, STDOUT_FILENO);
	dup2(stderr_fd, STDERR_FILENO);
}

static int parse_simple(simple_command_t *s, int level, command_t *father) {
	if (s == NULL || s->verb == NULL || s->verb->string == NULL)
		return shell_exit();

	char cwd[1024];

	if (getcwd(cwd, sizeof(cwd)) == NULL)
		return shell_exit();

	if (strcmp(s->verb->string, "exit") == 0 || strcmp(s->verb->string, "quit") == 0)
		return shell_exit();

	if (strcmp(s->verb->string, "cd") == 0)
		return shell_redirect(s), shell_cd(s->params);

	if (s->verb->next_part)
		return setenv(s->verb->string, get_word(s->verb->next_part->next_part), 1), true;

	pid_t pid = fork();
	int status;

	if (pid < 0)
		return shell_exit();

	if (pid > 0)
		return waitpid(pid, &status, 0), !status;

	int argc;
	char **argv = get_argv(s, &argc);

	s->in ? shell_redirect_stdin(s) : NULL;
	s->out ? shell_redirect_stdout(s) : NULL;
	s->err ? shell_redirect_stderr(s) : NULL;

	execvp(argv[0], argv);
	printf("Execution failed for '%s'\n", get_word(s->verb));

	return shell_exit();
}

static bool run_in_parallel(command_t *cmd1, command_t *cmd2, int level, command_t *father) {
	pid_t pid_cmd1 = fork();

	if (pid_cmd1 == 0)
		exit(parse_command(cmd1, level + 1, father));

	pid_t pid_cmd2 = fork();

	if (pid_cmd2 == 0)
		exit(parse_command(cmd2, level + 1, father));

	int status_cmd1;
	int status_cmd2;

	waitpid(pid_cmd1, &status_cmd1, 0);
	waitpid(pid_cmd2, &status_cmd2, 0);

	return status_cmd1 == 0 && status_cmd2 == 0;
}

static bool run_on_pipe(command_t *cmd1, command_t *cmd2, int level, command_t *father) {
	int fd[2];

	DIE(pipe(fd) < 0, "pipe failed");

	pid_t pid_cmd1 = fork();

	if (pid_cmd1 == 0) {
		dup2(fd[1], STDOUT_FILENO);

		close(fd[0]);
		close(fd[1]);

		exit(parse_command(cmd1, level + 1, father));
	}

	pid_t pid_cmd2 = fork();

	if (pid_cmd2 == 0) {
		dup2(fd[0], STDIN_FILENO);

		close(fd[0]);
		close(fd[1]);

		exit(parse_command(cmd2, level + 1, father));
	}

	close(fd[0]);
	close(fd[1]);

	int status_cmd1;
	int status_cmd2;

	waitpid(pid_cmd1, &status_cmd1, 0);
	waitpid(pid_cmd2, &status_cmd2, 0);

	return status_cmd2;
}

int parse_command(command_t *c, int level, command_t *father) {
	if (c == NULL)
		return shell_exit();

	if (c->op == OP_NONE)
		return parse_simple(c->scmd, level, father);

	int exit_status;

	switch (c->op) {
		case OP_SEQUENTIAL:
			exit_status = parse_command(c->cmd1, level + 1, c);
			exit_status = exit_status >= 0 ? parse_command(c->cmd2, level + 1, c) : exit_status;
			break;

		case OP_PARALLEL:
			exit_status = run_in_parallel(c->cmd1, c->cmd2, level, c);
			break;

		case OP_CONDITIONAL_NZERO:
			exit_status = parse_command(c->cmd1, level + 1, c);
			exit_status = exit_status == 0 ? parse_command(c->cmd2, level + 1, c) : exit_status;
			break;

		case OP_CONDITIONAL_ZERO:
			exit_status = parse_command(c->cmd1, level + 1, c);
			exit_status = exit_status != 0 ? parse_command(c->cmd2, level + 1, c) : exit_status;
			break;

		case OP_PIPE:
			exit_status = run_on_pipe(c->cmd1, c->cmd2, level, c);
			break;

		default:
			return shell_exit();
	}

	return exit_status;
}
