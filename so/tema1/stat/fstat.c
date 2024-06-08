#include <errno.h>
#include <internal/syscall.h>
#include <sys/stat.h>

int fstat(int fd, struct stat* st) {
	int response = syscall(__NR_fstat, fd, st);

	if (response >= 0)
		return response;

	errno = -response;
	return -1;
}
