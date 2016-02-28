/* xwrap.c - wrappers around existing library functions.
 *
 * Functions with the x prefix are wrappers that either succeed or kill the
 * program with an error message, but never return failure. They usually have
 * the same arguments and return value as the function they wrap.
 *
 * Copyright 2006 Rob Landley <rob@landley.net>
 */

#include "toys.h"
#include "xfuncs.h"

// strcpy and strncat with size checking. Size is the total space in "dest",
// including null terminator. Exit if there's not enough space for the string
// (including space for the null terminator), because silently truncating is
// still broken behavior. (And leaving the string unterminated is INSANE.)
void xstrncpy(char *dest, char *src, size_t size)
{
  if (strlen(src)+1 > size) error_exit("'%s' > %ld bytes", src, (long)size);
  strcpy(dest, src);
}

void xstrncat(char *dest, char *src, size_t size)
{
  long len = strlen(src);

  if (len+strlen(dest)+1 > size)
    error_exit("'%s%s' > %ld bytes", dest, src, (long)size);
  strcpy(dest+len, src);
}

void xexit(void)
{
  if (toys.rebound) longjmp(*toys.rebound, 1);
  if (fflush(NULL) || ferror(stdout))
    if (!toys.exitval) perror_msg("write");
  exit(toys.exitval);
}

// Die unless we can allocate memory.
void *xmalloc(size_t size)
{
  void *ret = malloc(size);
  if (!ret) error_exit("xmalloc");

  return ret;
}

// Die unless we can allocate prezeroed memory.
void *xzalloc(size_t size)
{
  void *ret = xmalloc(size);
  memset(ret, 0, size);
  return ret;
}

// Die unless we can change the size of an existing allocation, possibly
// moving it.  (Notice different arguments from libc function.)
void *xrealloc(void *ptr, size_t size)
{
  ptr = realloc(ptr, size);
  if (!ptr) error_exit("xrealloc");

  return ptr;
}

// Die unless we can allocate a copy of this many bytes of string.
char *xstrndup(char *s, size_t n)
{
  char *ret = strndup(s, ++n);

  if (!ret) error_exit("xstrndup");
  ret[--n] = 0;

  return ret;
}

// Die unless we can allocate a copy of this string.
char *xstrdup(char *s)
{
  return xstrndup(s, strlen(s));
}
