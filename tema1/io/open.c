#include <errno.h>
#include <fcntl.h>
#include <internal/syscall.h>
#include <stdarg.h>

int open(const char* filename, int flags, ...) {
	int response = syscall(__NR_open, filename, flags);

	if (response >= 0)
		return response;

	errno = -response;
	return -1;
}
