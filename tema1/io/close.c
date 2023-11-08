#include <errno.h>
#include <internal/syscall.h>
#include <stdarg.h>
#include <unistd.h>

int close(int fd) {
	return syscall(__NR_close, fd);
}
