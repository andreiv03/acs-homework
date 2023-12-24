#include "osmem.h"

#include <string.h>
#include <sys/mman.h>
#include <unistd.h>

#include "../utils/block_meta.h"

#define ALIGNMENT 8
#define ALIGN(size) ((size + ALIGNMENT - 1) & (~(ALIGNMENT - 1)))
#define BLOCK_META_SIZE ALIGN(sizeof(struct block_meta))
#define MMAP_THRESHOLD_MALLOC (128 * 1024)
#define MMAP_THRESHOLD_CALLOC (128 * 32)
#define SBRK_FAILED ((void *) -1)

struct block_meta *heap_start;
size_t MMAP_THRESHOLD = MMAP_THRESHOLD_MALLOC;
char is_first_brk_call = 1;

void *os_create_block(size_t size, size_t threshold_size) {
	struct block_meta *block = NULL;

	if (threshold_size < MMAP_THRESHOLD) {
		block = sbrk(is_first_brk_call ? MMAP_THRESHOLD_MALLOC : size + BLOCK_META_SIZE);
		DIE(block == SBRK_FAILED, "sbrk failed");
		block->size = size;
		block->status = STATUS_ALLOC;
	}

	if (threshold_size >= MMAP_THRESHOLD) {
		block = mmap(NULL, size + BLOCK_META_SIZE, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANON, -1, 0);
		DIE(block == MAP_FAILED, "mmap failed");
		block->size = size;
		block->status = STATUS_MAPPED;
	}

	return block;
}

void os_push_block(struct block_meta *block) {
	if (heap_start == NULL) {
		heap_start = block;
		return;
	}

	struct block_meta *aux = heap_start;

	while (aux->next != NULL)
		aux = aux->next;

	aux->next = block;
	block->prev = aux;
}

void os_remove_block(struct block_meta *block) {
	if (heap_start == block)
		heap_start = block->next;

	if (block->prev != NULL)
		block->prev->next = block->next;

	if (block->next != NULL)
		block->next->prev = block->prev;

	block->prev = NULL;
	block->next = NULL;
}

void os_coalesce_left_block(struct block_meta *block) {
	if (block->status != STATUS_FREE)
		return;

	if (block->prev == NULL || block->prev->status != STATUS_FREE)
		return;

	block->prev->size = block->prev->size + block->size + BLOCK_META_SIZE;
	os_remove_block(block);
}

void os_coalesce_right_block(struct block_meta *block) {
	if (block->status != STATUS_FREE)
		return;

	if (block->next == NULL || block->next->status != STATUS_FREE)
		return;

	block->size = block->size + block->next->size + BLOCK_META_SIZE;
	os_remove_block(block->next);
}

struct block_meta *os_search_best_block(size_t size) {
	if (heap_start == NULL)
		return NULL;

	struct block_meta *block = NULL;
	size_t block_size = ~1;

	struct block_meta *aux = heap_start;

	while (aux != NULL) {
		if (aux->status == STATUS_FREE && aux->size >= size && aux->size < block_size) {
			block = aux;
			block_size = aux->size;
		}

		aux = aux->next;
	}

	return block;
}

struct block_meta *os_split_block(struct block_meta *block, size_t size) {
	block->status = STATUS_ALLOC;

	if (block->size - size < BLOCK_META_SIZE + ALIGNMENT)
		return block;

	struct block_meta *new_block = (struct block_meta *) ((char *) block + size + BLOCK_META_SIZE);

	new_block->size = block->size - size - BLOCK_META_SIZE;
	new_block->status = STATUS_FREE;
	new_block->prev = block;
	new_block->next = NULL;

	if (block->next != NULL) {
		new_block->next = block->next;
		block->next->prev = new_block;
	}

	block->size = size;
	block->next = new_block;

	os_coalesce_left_block(new_block);
	os_coalesce_right_block(new_block);

	return block;
}

void *os_malloc(size_t size) {
	if (size == 0)
		return NULL;

	size = ALIGN(size);
	size_t threshold_size = MMAP_THRESHOLD == MMAP_THRESHOLD_MALLOC ? size : size + BLOCK_META_SIZE;

	if (threshold_size < MMAP_THRESHOLD && is_first_brk_call == 1) {
		struct block_meta *block = os_create_block(MMAP_THRESHOLD_MALLOC - BLOCK_META_SIZE, 0);

		block->status = STATUS_FREE;
		os_push_block(block);
		is_first_brk_call = 0;
	}

	struct block_meta *best_block = os_search_best_block(size);

	if (best_block != NULL && threshold_size < MMAP_THRESHOLD)
		return os_split_block(best_block, size) + 1;

	struct block_meta *heap_end = heap_start;

	while (heap_end != NULL && heap_end->next != NULL)
		heap_end = heap_end->next;

	while (heap_end != NULL && heap_end->status == STATUS_MAPPED)
		heap_end = heap_end->prev;

	if (heap_end != NULL && heap_end->status == STATUS_MAPPED)
		heap_end = NULL;

	if (heap_end != NULL && heap_end->status == STATUS_FREE && threshold_size < MMAP_THRESHOLD) {
		void *ptr = sbrk(size - heap_end->size);
		DIE(ptr == SBRK_FAILED, "sbrk failed");

		heap_end->size = size;
		heap_end->status = STATUS_ALLOC;

		return heap_end + 1;
	}

	struct block_meta *block = os_create_block(size, threshold_size);

	os_push_block(block);
	return block + 1;
}

void os_free(void *ptr) {
	if (ptr == NULL)
		return;

	struct block_meta *block = (struct block_meta *) ((char *) ptr - BLOCK_META_SIZE);

	if (block->status == STATUS_MAPPED) {
		os_remove_block(block);
		munmap(block, block->size + BLOCK_META_SIZE);
		return;
	}

	block->status = STATUS_FREE;

	os_coalesce_right_block(block);
	os_coalesce_left_block(block);
}

void *os_calloc(size_t nmemb, size_t size) {
	size_t total_size = ALIGN(nmemb * size);

	MMAP_THRESHOLD = MMAP_THRESHOLD_CALLOC;

	void *ptr = os_malloc(total_size);

	MMAP_THRESHOLD = MMAP_THRESHOLD_MALLOC;

	if (ptr != NULL)
		memset(ptr, 0, total_size);

	return ptr;
}

void *os_expand_block(struct block_meta *block, size_t size) {
	struct block_meta *heap_end = heap_start;

	while (heap_end != NULL && heap_end->next != NULL)
		heap_end = heap_end->next;

	if (heap_end == block) {
		void *ptr = sbrk(size - block->size);
		DIE(ptr == SBRK_FAILED, "sbrk failed");

		heap_end->size = size;
		heap_end->status = STATUS_ALLOC;

		return heap_end + 1;
	}

	if (block->next == NULL || block->next->status != STATUS_FREE)
		return NULL;

	if (block->size + block->next->size + BLOCK_META_SIZE < size)
		return NULL;

	if (block->size + block->next->size < size) {
		block->size = block->size + block->next->size + BLOCK_META_SIZE;
		os_remove_block(block->next);
		return block + 1;
	}

	size_t new_block_size = block->size + block->next->size - size;

	os_remove_block(block->next);

	struct block_meta *new_block = (struct block_meta *) ((char *) block + size + BLOCK_META_SIZE);

	new_block->size = new_block_size;
	new_block->status = STATUS_FREE;
	new_block->prev = block;
	new_block->next = NULL;

	if (block->next != NULL) {
		new_block->next = block->next;
		block->next->prev = new_block;
	}

	block->size = size;
	block->next = new_block;

	os_coalesce_left_block(new_block);
	os_coalesce_right_block(new_block);

	return block + 1;
}

void *os_realloc(void *ptr, size_t size) {
	size = ALIGN(size);

	if (ptr == NULL)
		return os_malloc(size);

	if (size == 0) {
		os_free(ptr);
		return NULL;
	}

	struct block_meta *block = (struct block_meta *) ((char *) ptr - BLOCK_META_SIZE);

	if (block->status == STATUS_FREE)
		return NULL;

	if (block->status == STATUS_MAPPED || size > MMAP_THRESHOLD) {
		void *new_ptr = os_malloc(size);

		memcpy(new_ptr, ptr, block->size > size ? size : block->size);
		os_free(ptr);
		return new_ptr;
	}

	if (block->size >= size)
		return os_split_block(block, size) + 1;

	void *new_ptr = os_expand_block(block, size);

	if (new_ptr)
		return new_ptr;

	new_ptr = os_malloc(size);
	memcpy(new_ptr, ptr, block->size);
	os_free(ptr);
	return new_ptr;
}
