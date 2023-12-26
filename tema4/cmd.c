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
	if (dir == NULL || dir->string == NULL)
		return false;

	if (chdir(dir->string) != 0)
		return false;

	return true;
}

static int shell_exit(void) {
	exit(SHELL_EXIT);
}

static char *process_path(const char *string, char *cwd, int store_cwd) {
	size_t length = 1024;
	char *path = calloc(length, sizeof(char));

	if (store_cwd == 0)
		snprintf(path, length, "%s", string);

	if (store_cwd == 1)
		snprintf(path, length, "%s/%s", cwd, string);

	return path;
}

static void shell_redirect(simple_command_t *s, char *cwd, int store_cwd) {
	int fd;
	int open_flags;

	if (s->in != NULL) {
		char *in_path = process_path(s->in->string, cwd, store_cwd);

		if (s->in->next_part != NULL)
			strcat(in_path, get_word(s->in->next_part));

		fd = open(in_path, O_RDONLY);
		DIE(fd < 0, "open failed");

		dup2(fd, STDIN_FILENO);
		close(fd);

		free(in_path);
	}

	if (s->out != NULL) {
		char *out_path = process_path(s->out->string, cwd, store_cwd);

		if (s->out->next_part != NULL)
			strcat(out_path, get_word(s->out->next_part));

		if (s->err != NULL || s->io_flags == IO_OUT_APPEND)
			open_flags = O_WRONLY | O_CREAT | O_APPEND;
		else
			open_flags = O_WRONLY | O_CREAT | O_TRUNC;

		fd = open(out_path, open_flags, 0644);
		DIE(fd < 0, "open failed");

		dup2(fd, STDOUT_FILENO);
		close(fd);

		free(out_path);
	}

	if (s->err != NULL) {
		char *err_path = process_path(s->err->string, cwd, store_cwd);

		if (s->err->next_part != NULL)
			strcat(err_path, get_word(s->err->next_part));

		if (s->out != NULL || s->io_flags == IO_REGULAR)
			open_flags = O_WRONLY | O_CREAT | O_TRUNC;
		else
			open_flags = O_WRONLY | O_CREAT | O_APPEND;

		fd = open(err_path, open_flags, 0644);
		DIE(fd < 0, "open failed");

		dup2(fd, STDERR_FILENO);
		close(fd);

		free(err_path);
	}
}

static int parse_simple(simple_command_t *s, int level, command_t *father) {
	if (s == NULL || s->verb == NULL || s->verb->string == NULL)
		return shell_exit();

	char cwd[1024];

	if (getcwd(cwd, sizeof(cwd)) == NULL)
		return shell_exit();

	if (strcmp(s->verb->string, "exit") == 0 || strcmp(s->verb->string, "quit") == 0)
		return shell_exit();

	if (strcmp(s->verb->string, "cd") == 0) {
		int stdin_fd = dup(STDIN_FILENO);
		int stdout_fd = dup(STDOUT_FILENO);
		int stderr_fd = dup(STDERR_FILENO);

		shell_redirect(s, cwd, 1);

		dup2(stdin_fd, STDIN_FILENO);
		close(stdin_fd);

		dup2(stdout_fd, STDOUT_FILENO);
		close(stdout_fd);

		dup2(stderr_fd, STDERR_FILENO);
		close(stderr_fd);

		return shell_cd(s->params);
	}

	if (s->verb->next_part != NULL) {
		char *value = get_word(s->verb->next_part->next_part);

		setenv(s->verb->string, value, 1);
		free(value);

		return true;
	}

	pid_t pid = fork();

	if (pid < 0)
		return shell_exit();

	if (pid > 0) {
		int status;

		waitpid(pid, &status, 0);
		return status ? 0 : 1;
	}

	int argc;
	char **argv = get_argv(s, &argc);

	shell_redirect(s, cwd, 0);

	execvp(argv[0], argv);
	printf("Execution failed for '%s'\n", get_word(s->verb));

	for (int index = 0; index <= argc; ++index)
		free(argv[index]);
	free(argv);

	return shell_exit();
}

static bool run_in_parallel(command_t *cmd1, command_t *cmd2, int level, command_t *father) {
	pid_t pid_cmd1 = fork();

	if (pid_cmd1 == 0)
		exit(parse_command(cmd1, level + 1, father));

	pid_t pid_cmd2 = fork();

	if (pid_cmd2 == 0)
		exit(parse_command(cmd2, level + 1, father));

	int status_cmd1, status_cmd2;

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

	int status_cmd1, status_cmd2;

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
			return SHELL_EXIT;
	}

	return exit_status;
}
