#include <internal/essentials.h>
#include <internal/mm/mem_list.h>
#include <internal/types.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>

void* malloc(size_t size) {
	int prot = PROT_READ | PROT_WRITE;
	int flags = MAP_ANONYMOUS | MAP_PRIVATE;
	void* pointer = mmap(NULL, size, prot, flags, -1, 0);

	if (pointer == MAP_FAILED)
		return NULL;

	mem_list_add(pointer, size);
	return pointer;
}

void* calloc(size_t nmemb, size_t size) {
	int prot = PROT_READ | PROT_WRITE;
	int flags = MAP_ANONYMOUS | MAP_PRIVATE;
	void* pointer = mmap(NULL, nmemb * size, prot, flags, -1, 0);

	if (pointer == MAP_FAILED)
		return NULL;

	memset(pointer, 0, nmemb * size);
	mem_list_add(pointer, nmemb * size);
	return pointer;
}

void free(void* ptr) {
	struct mem_list* item = mem_list_find(ptr);

	if (item == NULL)
		return;

	size_t length = item->len;

	mem_list_del(ptr);
	munmap(ptr, length);
}

void* realloc(void* ptr, size_t size) {
	int prot = PROT_READ | PROT_WRITE;
	int flags = MAP_ANONYMOUS | MAP_PRIVATE;
	void* pointer = mmap(NULL, size, prot, flags, -1, 0);

	if (pointer == MAP_FAILED)
		return NULL;

	memcpy(pointer, ptr, size);
	return pointer;
}

void* reallocarray(void* ptr, size_t nmemb, size_t size) {
	int prot = PROT_READ | PROT_WRITE;
	int flags = MAP_ANONYMOUS | MAP_PRIVATE;
	void* pointer = mmap(NULL, nmemb * size, prot, flags, -1, 0);

	if (pointer == MAP_FAILED)
		return NULL;

	memcpy(pointer, ptr, nmemb * size);
	return pointer;
}
