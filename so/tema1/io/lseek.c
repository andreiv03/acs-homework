#include <errno.h>
#include <internal/syscall.h>
#include <unistd.h>

off_t lseek(int fd, off_t offset, int whence) {
	off_t response = (off_t)syscall(__NR_lseek, fd, offset, whence);

	if (response >= 0)
		return response;

	errno = -response;
	return -1;
}
