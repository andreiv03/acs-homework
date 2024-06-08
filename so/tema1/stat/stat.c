#include <errno.h>
#include <fcntl.h>
#include <internal/syscall.h>
#include <sys/stat.h>

int stat(const char* restrict path, struct stat* restrict buf) {
	int response = syscall(__NR_stat, path, buf);

	if (response >= 0)
		return response;

	errno = -response;
	return -1;
}
