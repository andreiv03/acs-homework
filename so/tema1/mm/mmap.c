#include <errno.h>
#include <internal/syscall.h>
#include <sys/mman.h>

void* mmap(void* addr, size_t length, int prot, int flags, int fd, off_t offset) {
	ssize_t response = syscall(__NR_mmap, addr, length, prot, flags, fd, offset);

	if (response >= 0)
		return (void*)response;

	errno = -response;
	return MAP_FAILED;
}

void* mremap(void* old_address, size_t old_size, size_t new_size, int flags) {
	ssize_t response = syscall(__NR_mremap, old_address, old_size, new_size, flags);

	if (response >= 0)
		return (void*)response;

	errno = -response;
	return MAP_FAILED;
}

int munmap(void* addr, size_t length) {
	int response = syscall(__NR_munmap, addr, length);

	if (response >= 0)
		return response;

	errno = -response;
	return -1;
}
