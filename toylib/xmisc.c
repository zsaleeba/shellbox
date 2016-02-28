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


void xchroot(char *path)
{
  if (chroot(path)) error_exit("chroot '%s'", path);
  xchdir("/");
}

struct passwd *xgetpwuid(uid_t uid)
{
  struct passwd *pwd = getpwuid(uid);
  if (!pwd) error_exit("bad uid %ld", (long)uid);
  return pwd;
}

struct group *xgetgrgid(gid_t gid)
{
  struct group *group = getgrgid(gid);

  if (!group) perror_exit("gid %ld", (long)gid);
  return group;
}

struct passwd *xgetpwnamid(char *user)
{
  struct passwd *up = getpwnam(user);
  uid_t uid;

  if (!up) {
    char *s = 0;

    uid = estrtol(user, &s, 10);
    if (!errno && s && !*s) up = getpwuid(uid);
  }
  if (!up) perror_exit("user '%s'", user);

  return up;
}

struct group *xgetgrnamid(char *group)
{
  struct group *gr = getgrnam(group);
  gid_t gid;

  if (!gr) {
    char *s = 0;

    gid = estrtol(group, &s, 10);
    if (!errno && s && !*s) gr = getgrgid(gid);
  }
  if (!gr) perror_exit("group '%s'", group);

  return gr;
}

struct passwd *xgetpwnam(char *name)
{
  struct passwd *up = getpwnam(name);

  if (!up) perror_exit("user '%s'", name);
  return up;
}

struct group *xgetgrnam(char *name)
{
  struct group *gr = getgrnam(name);

  if (!gr) perror_exit("group '%s'", name);
  return gr;
}

// setuid() can fail (for example, too many processes belonging to that user),
// which opens a security hole if the process continues as the original user.

void xsetuser(struct passwd *pwd)
{
  if (initgroups(pwd->pw_name, pwd->pw_gid) || setgid(pwd->pw_uid)
      || setuid(pwd->pw_uid)) perror_exit("xsetuser '%s'", pwd->pw_name);
}


// Compile a regular expression into a regex_t
void xregcomp(regex_t *preg, char *regex, int cflags)
{
  int rc = regcomp(preg, regex, cflags);

  if (rc) {
    regerror(rc, preg, libbuf, sizeof(libbuf));
    error_exit("xregcomp: %s", libbuf);
  }
}


char *xtzset(char *new)
{
  char *tz = getenv("TZ");

  if (tz) tz = xstrdup(tz);
  if (setenv("TZ", new, 1)) perror_exit("setenv");
  tzset();

  return tz;
}

// Set a signal handler
void xsignal(int signal, void *handler)
{
  struct sigaction *sa = (void *)libbuf;

  memset(sa, 0, sizeof(struct sigaction));
  sa->sa_handler = handler;

  if (sigaction(signal, sa, 0)) perror_exit("xsignal %d", signal);
}
