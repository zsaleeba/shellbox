/* xwrap.c - wrappers around existing library functions.
 *
 * Functions with the x prefix are wrappers that either succeed or kill the
 * program with an error message, but never return failure. They usually have
 * the same arguments and return value as the function they wrap.
 *
 * Copyright 2006 Rob Landley <rob@landley.net>
 */

#include <assert.h>

#include "toys.h"
#include "xfuncs.h"


// This is called through the XVFORK macro because parent/child of vfork
// share a stack, so child returning from a function would stomp the return
// address parent would need. Solution: make vfork() an argument so processes
// diverge before function gets called.
pid_t xvforkwrap(pid_t pid)
{
  if (pid == -1) perror_exit("vfork");

  // Signal to xexec() and friends that we vforked so can't recurse
  toys.stacktop = 0;

  return pid;
}

pid_t xvfork()
{
  assert(/* vfork() is disabled. */ 0);
  return 0;
}

// Die unless we can exec argv[] (or run builtin command).  Note that anything
// with a path isn't a builtin, so /bin/sh won't match the builtin sh.
void xexec(char **argv)
{
  // Only recurse to builtin when we have multiplexer and !vfork context.
  if (CFG_TOYBOX && !CFG_TOYBOX_NORECURSE && toys.stacktop)
  {
     struct toy_list *toy = toy_find(argv[0]);
     if (!toy)
     {
        fprintf(stderr, "exec %s: not found\n", argv[0]);
        return;
     }
     
     toy_init(toy, argv);
     toy_run(toy, argv);
  }
  else
  {
    assert(/* exec is disabled. */ 0);
    execvp(argv[0], argv);
  
    perror_msg("exec %s", argv[0]);
    toys.exitval = 127;
    if (!CFG_TOYBOX_FORK) _exit(toys.exitval);
    xexit();
  }
}

int xexecl(const char *path, const char *arg, ...)
{
  (void)path;
  (void)arg;
  assert(/* exec is disabled. */ 0);
  errno = ENOSYS;
  return -1;
}

int xexeclp(const char *file, const char *arg, ...)
{
  (void)file;
  (void)arg;
  assert(/* exec is disabled. */ 0);
  errno = ENOSYS;
  return -1;
}

int xexecv(const char *path, char *const argv[])
{
  (void)path;
  (void)argv;
  assert(/* exec is disabled. */ 0);
  errno = ENOSYS;
  return -1;
}

int xexecvp(const char *file, char *const argv[])
{
  (void)file;
  (void)argv;
  assert(/* exec is disabled. */ 0);
  errno = ENOSYS;
  return -1;
}

// Spawn child process, capturing stdin/stdout.
// argv[]: command to exec. If null, child re-runs original program with
//         toys.stacktop zeroed.
// pipes[2]: stdin, stdout of new process, only allocated if zero on way in,
//           pass NULL to skip pipe allocation entirely.
// return: pid of child process
pid_t xpopen_both(char **argv, int *pipes)
{
  int cestnepasun[4], pid;

  // Make the pipes? Note this won't set either pipe to 0 because if fds are
  // allocated in order and if fd0 was free it would go to cestnepasun[0]
  if (pipes) {
    for (pid = 0; pid < 2; pid++) {
      if (pipes[pid] != 0) continue;
      if (pipe(cestnepasun+(2*pid))) perror_exit("pipe");
      pipes[pid] = cestnepasun[pid+1];
    }
  }

  // Child process.
  if (!(pid = CFG_TOYBOX_FORK ? xfork() : XVFORK())) {
    // Dance of the stdin/stdout redirection.
    if (pipes) {
      // if we had no stdin/out, pipe handles could overlap, so test for it
      // and free up potentially overlapping pipe handles before reuse
      if (pipes[1] != -1) close(cestnepasun[2]);
      if (pipes[0] != -1) {
        close(cestnepasun[1]);
        if (cestnepasun[0]) {
          dup2(cestnepasun[0], 0);
          close(cestnepasun[0]);
        }
      }
      if (pipes[1] != -1) {
        dup2(cestnepasun[3], 1);
        dup2(cestnepasun[3], 2);
        if (cestnepasun[3] > 2 || !cestnepasun[3]) close(cestnepasun[3]);
      }
    }
    if (argv) xexec(argv);

    // In fork() case, force recursion because we know it's us.
    if (CFG_TOYBOX_FORK) {
      toy_init(toys.which, toys.argv);
      toys.stacktop = 0;
      toys.which->toy_main();
      xexit();
    // In vfork() case, exec /proc/self/exe with high bit of first letter set
    // to tell main() we reentered.
    } else {
      char *s = "/proc/self/exe";

      // We did a nommu-friendly vfork but must exec to continue.
      // setting high bit of argv[0][0] to let new process know
      **toys.argv |= 0x80;
      xexecv(s, toys.argv);
      perror_msg(s);

      _exit(127);
    }
  }

  // Parent process
  if (!CFG_TOYBOX_FORK) **toys.argv &= 0x7f;
  if (pipes) {
    if (pipes[0] != -1) close(cestnepasun[0]);
    if (pipes[1] != -1) close(cestnepasun[3]);
  }

  return pid;
}

// Wait for child process to exit, then return adjusted exit code.
int xwaitpid(pid_t pid)
{
  int status;

  while (-1 == waitpid(pid, &status, 0) && errno == EINTR);

  return WIFEXITED(status) ? WEXITSTATUS(status) : WTERMSIG(status)+127;
}

int xpclose_both(pid_t pid, int *pipes)
{
  if (pipes) {
    close(pipes[0]);
    close(pipes[1]);
  }

  return xwaitpid(pid);
}

// Wrapper to xpopen with a pipe for just one of stdin/stdout
pid_t xpopen(char **argv, int *pipe, int stdout_)
{
  int pipes[2], pid;

  pipes[!stdout_] = -1;
  pipes[!!stdout_] = 0;
  pid = xpopen_both(argv, pipes);
  *pipe = pid ? pipes[!!stdout_] : -1;

  return pid;
}

int xpclose(pid_t pid, int pipe)
{
  close(pipe);

  return xpclose_both(pid, 0);
}

// Call xpopen and wait for it to finish, keeping existing stdin/stdout.
int xrun(char **argv)
{
  return xpclose_both(xpopen_both(argv, 0), 0);
}

