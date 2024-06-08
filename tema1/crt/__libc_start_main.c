#include <internal/mm/mem_list.h>
#include <internal/types.h>

static void init(void) {
	mem_list_init();
}

static void cleanup(void) {
	mem_list_cleanup();
}

int __libc_start_main(int (*main_fn)(void)) {
	int exit_code;

	init();
	exit_code = main_fn();
	cleanup();

	return exit_code;
}
