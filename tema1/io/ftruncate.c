#include <errno.h>
#include <internal/syscall.h>
#include <unistd.h>

int ftruncate(int fd, off_t length) {
	int response = syscall(__NR_ftruncate, fd, length);

	if (response >= 0)
		return response;

	errno = -response;
	return -1;
}
