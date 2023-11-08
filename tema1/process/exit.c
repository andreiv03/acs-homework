#include <internal/syscall.h>
#include <stdlib.h>

long exit(long exit_code) {
	return syscall(__NR_exit, exit_code);
}
