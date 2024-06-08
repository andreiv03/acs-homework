#include <errno.h>
#include <fcntl.h>
#include <internal/syscall.h>
#include <internal/types.h>
#include <sys/stat.h>
#include <unistd.h>

#define makedev(x, y) (           \
		(((x)&0xfffff000ULL) << 32) | \
		(((x)&0x00000fffULL) << 8) |  \
		(((y)&0xffffff00ULL) << 12) | \
		(((y)&0x000000ffULL)))

struct statx {
	uint32_t stx_mask;
	uint32_t stx_blksize;
	uint64_t stx_attributes;
	uint32_t stx_nlink;
	uint32_t stx_uid;
	uint32_t stx_gid;
	uint16_t stx_mode;
	uint16_t pad1;
	uint64_t stx_ino;
	uint64_t stx_size;
	uint64_t stx_blocks;
	uint64_t stx_attributes_mask;
	struct {
		int64_t tv_sec;
		uint32_t tv_nsec;
		int32_t pad;
	} stx_atime, stx_btime, stx_ctime, stx_mtime;
	uint32_t stx_rdev_major;
	uint32_t stx_rdev_minor;
	uint32_t stx_dev_major;
	uint32_t stx_dev_minor;
	uint64_t spare[14];
};
