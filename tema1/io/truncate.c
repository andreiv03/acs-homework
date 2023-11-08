#include <errno.h>
#include <internal/syscall.h>
#include <unistd.h>

int truncate(const char* path, off_t length) {
	int response = syscall(__NR_truncate, path, length);

	if (response >= 0)
		return response;

	errno = -response;
	return -1;
}
