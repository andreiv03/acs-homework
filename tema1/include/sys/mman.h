#ifndef __SYS_MMAN_H__
#define __SYS_MMAN_H__ 1

#ifdef __cplusplus
extern "C" {
#endif

#include <internal/types.h>

#define PROT_READ 0x1
#define PROT_WRITE 0x2
#define PROT_EXEC 0x4
#define PROT_NONE 0x0

#define MAP_SHARED 0x01
#define MAP_PRIVATE 0x02
#define MAP_ANONYMOUS 0x20
#define MAP_ANON MAP_ANONYMOUS

#define MREMAP_MAYMOVE 1

#define MAP_FAILED ((void*)-1)

void* mmap(void* addr, size_t length, int prot, int flags, int fd, off_t offset);
void* mremap(void* old_address, size_t old_size, size_t new_size, int flags);
int munmap(void* addr, size_t length);

#ifdef __cplusplus
}
#endif

#endif
