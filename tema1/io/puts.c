#include <errno.h>
#include <internal/syscall.h>
#include <string.h>
#include <unistd.h>

int puts(const char* s) {
	int response = syscall(__NR_write, 1, s, strlen(s));

	if (response < 0) {
		errno = -response;
		return -1;
	}

	response = syscall(__NR_write, 1, "\n", 1);

	if (response < 0) {
		errno = -response;
		return -1;
	}

	return response;
}
