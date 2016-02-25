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
#include <sys/time.h>
#include <utime.h>
#include <errno.h>
#include <unistd.h>

#include "xfuncs.h"



int xutimensat(int dirfd, const char *pathname, const struct timespec times[2], int flags)
{
	int cfd, ret, error, i;
	struct timeval tvs[2];

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

	for (i = 0; i < 2; i++)
	{
		tvs[i].tv_sec = times[i].tv_sec;
		tvs[i].tv_usec = times[i].tv_nsec / 1000;
	}

	ret = utimes(pathname, tvs);
	
	error = errno;
	(void)fchdir(cfd);
	(void)close(cfd);
	errno = error;
	return ret;
}

int xfutimens(int fd, const struct timespec times[2])
{
	errno = EINVAL;
	return -1;
}
