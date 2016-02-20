/*-
 * Copyright (c) 2012 The FreeBSD Foundation
 * All rights reserved.
 *
 * This software was developed by Pawel Jakub Dawidek under sponsorship from
 * the FreeBSD Foundation.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHORS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHORS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdarg.h>
#include <errno.h>

#include "xfuncs.h"

#ifndef PATH_MAX
#define PATH_MAX 8192
#endif

int
xfaccessat(int fd, const char *path, int mode, int flag)
{
	int cfd, error, ret;

	if (flag == AT_EACCESS) {
		errno = EINVAL;
		return (-1);
	}

	cfd = open(".", O_RDONLY
#ifdef O_DIRECTORY
		   | O_DIRECTORY
#endif
		   );
	if (cfd == -1)
		return (-1);

	if (fchdir(fd) == -1) {
		error = errno;
		(void)close(cfd);
		errno = error;
		return (-1);
	}

	ret = access(path, mode);

	error = errno;
	(void)fchdir(cfd);
	(void)close(cfd);
	errno = error;
	return (ret);
}

int
xfstatat(int fd, const char *path, struct stat *buf, int flag)
{
	int cfd, error, ret;

	cfd = open(".", O_RDONLY
#ifdef O_DIRECTORY
		   | O_DIRECTORY
#endif
		   );
	if (cfd == -1)
		return (-1);

	if (fchdir(fd) == -1) {
		error = errno;
		(void)close(cfd);
		errno = error;
		return (-1);
	}

	if (flag == AT_SYMLINK_NOFOLLOW)
		ret = lstat(path, buf);
	else
		ret = stat(path, buf);

	error = errno;
	(void)fchdir(cfd);
	(void)close(cfd);
	errno = error;
	return (ret);
}

int xmkdirat(int dirfd, const char *pathname, mode_t mode)
{
        int cfd;
        int error;
        int ret;

        if (*pathname == '/'
#ifdef AT_FDCWD
                        || dirfd == AT_FDCWD
#endif
                        )
        {
                /* absolute path or relative to current directory. just use mkdir. */
                return mkdir(pathname, mode);
        }
        else
        {
                /* relative path. cd to the directory, then do the mkdir. restore the old directory afterwards. */
                cfd = open(".", O_RDONLY
#ifdef O_DIRECTORY
                           | O_DIRECTORY
#endif
                           );
                if (cfd == -1)
                        return -1;

                /* change to the target directory. */
                if (fchdir(dirfd) == -1)
                {
                        error = errno;
                        (void)close(cfd);
                        errno = error;
                        return -1;
                }
                
                /* make the new directory. */
                ret = mkdir(pathname, mode);
                
                /* return to the original directory. */
                error = errno;
                fchdir(cfd);
                close(cfd);
                errno = error;
                return ret;
        }
}

int
xopenat(int fd, const char *path, int flags, ...)
{
	int cfd, ffd, error;

	cfd = open(".", O_RDONLY 
#ifdef O_DIRECTORY
		   | O_DIRECTORY
#endif
		   );
	if (cfd == -1)
		return (-1);

	if (fchdir(fd) == -1) {
		error = errno;
		(void)close(cfd);
		errno = error;
		return (-1);
	}

	if ((flags & O_CREAT) != 0) {
		va_list ap;
		int mode;

		va_start(ap, flags);
		mode = va_arg(ap, int);
		va_end(ap);

		ffd = open(path, flags, mode);
	} else {
		ffd = open(path, flags);
	}

	error = errno;
	(void)fchdir(cfd);
	(void)close(cfd);
	errno = error;
	return (ffd);
}

int
xfchmodat(int dirfd, const char *pathname, mode_t mode, int flags)
{
	int cfd, ret, error;

	cfd = open(".", O_RDONLY 
#ifdef O_DIRECTORY
		   | O_DIRECTORY
#endif
		   );
	if (cfd == -1)
		return (-1);

	if (fchdir(dirfd) == -1) {
		error = errno;
		(void)close(cfd);
		errno = error;
		return (-1);
	}

	ret = chmod(pathname, mode);

	error = errno;
	(void)fchdir(cfd);
	(void)close(cfd);
	errno = error;
	return ret;
}

int
xreadlinkat(int dirfd, const char *pathname, char *buf, size_t bufsiz)
{
	int cfd, ret, error;

	cfd = open(".", O_RDONLY 
#ifdef O_DIRECTORY
		   | O_DIRECTORY
#endif
		   );
	if (cfd == -1)
		return (-1);

	if (fchdir(dirfd) == -1) {
		error = errno;
		(void)close(cfd);
		errno = error;
		return (-1);
	}

	ret = readlink(pathname, buf, bufsiz);

	error = errno;
	(void)fchdir(cfd);
	(void)close(cfd);
	errno = error;
	return ret;
}

int 
xunlinkat(int dirfd, const char *pathname, int flags)
{
	int cfd, ret, error;

	cfd = open(".", O_RDONLY 
#ifdef O_DIRECTORY
		   | O_DIRECTORY
#endif
		   );
	if (cfd == -1)
		return (-1);

	if (fchdir(dirfd) == -1) {
		error = errno;
		(void)close(cfd);
		errno = error;
		return (-1);
	}

	if (flags & AT_REMOVEDIR)
		ret = rmdir(pathname);
	else
		ret = unlink(pathname);

	error = errno;
	(void)fchdir(cfd);
	(void)close(cfd);
	errno = error;
	return ret;
}

int 
xfchownat(int dirfd, const char *pathname, uid_t owner, gid_t group, int flags)
{
	int cfd, ret, error;

	cfd = open(".", O_RDONLY 
#ifdef O_DIRECTORY
		   | O_DIRECTORY
#endif
		   );
	if (cfd == -1)
		return (-1);

	if (fchdir(dirfd) == -1) {
		error = errno;
		(void)close(cfd);
		errno = error;
		return (-1);
	}

#ifdef AT_EMPTY_PATH
	if (flags & AT_EMPTY_PATH)
		ret = fchown(dirfd, owner, group);
	else 
#endif
	if (flags & AT_SYMLINK_NOFOLLOW)
		ret = lchown(pathname, owner, group);
	else
		ret = chown(pathname, owner, group);

	error = errno;
	(void)fchdir(cfd);
	(void)close(cfd);
	errno = error;
	return ret;
}

int 
xsymlinkat(const char *oldpath, int newdirfd, const char *newpath)
{
	int cfd, ret, error;

	cfd = open(".", O_RDONLY 
#ifdef O_DIRECTORY
		   | O_DIRECTORY
#endif
		   );
	if (cfd == -1)
		return (-1);

	if (fchdir(newdirfd) == -1) {
		error = errno;
		(void)close(cfd);
		errno = error;
		return (-1);
	}

	ret = symlink(oldpath, newpath);

	error = errno;
	(void)fchdir(cfd);
	(void)close(cfd);
	errno = error;
	return ret;
}

int 
xmknodat(int dirfd, const char *pathname, mode_t mode, dev_t dev)
{
	int cfd, ret, error;

	cfd = open(".", O_RDONLY 
#ifdef O_DIRECTORY
		   | O_DIRECTORY
#endif
		   );
	if (cfd == -1)
		return (-1);

	if (fchdir(dirfd) == -1) {
		error = errno;
		(void)close(cfd);
		errno = error;
		return (-1);
	}

	ret = mknod(pathname, mode, dev);

	error = errno;
	(void)fchdir(cfd);
	(void)close(cfd);
	errno = error;
	return ret;
}

int 
xlinkat(int olddirfd, const char *oldpath, int newdirfd, const char *newpath, int flags)
{
	/* implement this later. */
	errno = EINVAL;
	return -1;
}
