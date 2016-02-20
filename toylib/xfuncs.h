#ifndef	XFUNCS_H
#define	XFUNCS_H

#include <fcntl.h>

#ifndef AT_SYMLINK_NOFOLLOW
#define	AT_SYMLINK_NOFOLLOW 2
#endif

#ifndef AT_EACCESS
#define	AT_EACCESS	0x01
#endif


/* prototypes for functions which may not be available in an embedded os. */
int xfstatat(int fd, const char *path, struct stat *buf, int flag);
int xfaccessat(int fd, const char *path, int mode, int flag);
int xmkdirat(int dirfd, const char *pathname, mode_t mode);
int xopenat(int fd, const char *path, int flags, ...);
int xfchmodat(int dirfd, const char *pathname, mode_t mode, int flags);
int xreadlinkat(int dirfd, const char *pathname, char *buf, size_t bufsiz);
int xunlinkat(int dirfd, const char *pathname, int flags);
int xfchownat(int dirfd, const char *pathname, uid_t owner, gid_t group, int flags);
int xsymlinkat(const char *oldpath, int newdirfd, const char *newpath);
int xmknodat(int dirfd, const char *pathname, mode_t mode, dev_t dev);

#endif	/* XFUNCS_H */
