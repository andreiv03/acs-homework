#ifndef __SYS_STAT_H__
#define __SYS_STAT_H__ 1

#ifdef __cplusplus
extern "C" {
#endif

#include <internal/types.h>

#define STATX_TYPE 0x00000001U
#define STATX_MODE 0x00000002U
#define STATX_NLINK 0x00000004U
#define STATX_UID 0x00000008U
#define STATX_GID 0x00000010U
#define STATX_ATIME 0x00000020U
#define STATX_MTIME 0x00000040U
#define STATX_CTIME 0x00000080U
#define STATX_INO 0x00000100U
#define STATX_SIZE 0x00000200U
#define STATX_BLOCKS 0x00000400U
#define STATX_BASIC_STATS 0x000007ffU
#define STATX_BTIME 0x00000800U
#define STATX_MNT_ID 0x00001000U

#define __S_IFMT 0170000

#define __S_IFDIR 0040000
#define __S_IFCHR 0020000
#define __S_IFBLK 0060000
#define __S_IFREG 0100000
#define __S_IFIFO 0010000
#define __S_IFLNK 0120000
#define __S_IFSOCK 0140000

#define AT_EMPTY_PATH 0x1000

struct stat {
	uint64_t st_dev;
	uint64_t st_ino;
	uint64_t st_nlink;

	unsigned int st_mode;
	unsigned int st_uid;
	unsigned int st_gid;
	unsigned int __pad0;
	uint64_t st_rdev;
	int64_t st_size;
	int64_t st_blksize;
	int64_t st_blocks;

	uint64_t st_atime;
	uint64_t st_atime_nsec;
	uint64_t st_mtime;
	uint64_t st_mtime_nsec;
	uint64_t st_ctime;
	uint64_t st_ctime_nsec;
	int64_t __unused[3];
};

int stat(const char* __restrict, struct stat* __restrict);
int fstat(int fd, struct stat* st);
int fstatat(int fd, const char* restrict path, struct stat* restrict st, int flag);

#ifdef __cplusplus
}
#endif

#endif
