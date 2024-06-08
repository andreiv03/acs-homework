#include <errno.h>
#include <internal/syscall.h>
#include <time.h>

int nanosleep(const struct timespec* req, struct timespec* rem) {
	int response = syscall(__NR_nanosleep, req, rem);

	if (response >= 0)
		return response;

	errno = -response;
	return -1;
}

unsigned int sleep(unsigned int seconds) {
	struct timespec req = {seconds, 0};
	struct timespec rem = {0, 0};

	if (nanosleep(&req, &rem) == -1)
		return rem.tv_sec;

	return 0;
}
