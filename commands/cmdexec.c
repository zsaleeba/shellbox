/* Toybox infrastructure.
 *
 * Copyright 2006 Rob Landley <rob@landley.net>
 */

#include "toys.h"

// Populate toy_list[].

#undef NEWTOY
#undef OLDTOY
#define NEWTOY(name, opts, flags) {#name, name##_main, opts, flags},
#define OLDTOY(name, oldname, flags) \
  {#name, oldname##_main, OPTSTR_##oldname, flags},

struct toy_list toy_list[] = {
#include "geninc/newtoys.h"
};

// global context for this command.

struct toy_context toys;
union global_union this;
char toybuf[4096], libbuf[4096];

struct toy_list *toy_find(char *name)
{
  int top, bottom, middle;

  if (!CFG_TOYBOX) return 0;

  // If the name starts with "toybox" accept that as a match.  Otherwise
  // skip the first entry, which is out of order.

  if (!strncmp(name,"toybox",6)) return toy_list;
  bottom = 1;

  // Binary search to find this command.

  top = ARRAY_LEN(toy_list)-1;
  for (;;) {
    int result;

    middle = (top+bottom)/2;
    if (middle<bottom || middle>top) return NULL;
    result = strcmp(name,toy_list[middle].name);
    if (!result) return toy_list+middle;
    if (result<0) top=--middle;
    else bottom = ++middle;
  }
}

// Figure out whether or not anything is using the option parsing logic,
// because the compiler can't figure out whether or not to optimize it away
// on its' own.  NEED_OPTIONS becomes a constant allowing if() to optimize
// stuff out via dead code elimination.

#undef NEWTOY
#undef OLDTOY
#define NEWTOY(name, opts, flags) opts ||
#define OLDTOY(name, oldname, flags) OPTSTR_##oldname ||
static const int NEED_OPTIONS =
#include "geninc/newtoys.h"
0;  // Ends the opts || opts || opts...

// Setup toybox global state for this command.
static void toy_singleinit(struct toy_list *which, char *argv[])
{
  toys.which = which;
  toys.argv = argv;

  if (CFG_TOYBOX_I18N) setlocale(LC_ALL, "C"+!!(which->flags & TOYFLAG_LOCALE));

  if (CFG_TOYBOX_HELP_DASHDASH && argv[1] && !strcmp(argv[1], "--help")) {
    if (CFG_TOYBOX && toys.which == toy_list && toys.argv[2])
      if (!(toys.which = toy_find(toys.argv[2]))) return;
    show_help(stdout);
    xexit();
  }

  if (NEED_OPTIONS && which->options) get_optflags();
  else {
    toys.optargs = argv+1;
    for (toys.optc = 0; toys.optargs[toys.optc]; toys.optc++);
  }
  toys.old_umask = umask(0);
  if (!(which->flags & TOYFLAG_UMASK)) umask(toys.old_umask);
  toys.signalfd--;
  toys.toycount = ARRAY_LEN(toy_list);
}

// Full init needed by multiplexer or reentrant calls, calls singleinit at end
void toy_init(struct toy_list *which, char *argv[])
{
  // Drop permissions for non-suid commands.

  if (CFG_TOYBOX_SUID) {
    uid_t uid = getuid(), euid = geteuid();

    if (!(which->flags & TOYFLAG_STAYROOT)) {
      if (uid != euid) {
        if (!setuid(uid)) perror_exit("setuid %d->%d", euid, uid); // drop root
        else euid = uid;
      }
    } else if (CFG_TOYBOX_DEBUG && uid && which != toy_list)
      error_msg("Not installed suid root");

    if ((which->flags & TOYFLAG_NEEDROOT) && euid) help_exit("Not root");
  }

  // Free old toys contents (to be reentrant), but leave rebound if any
  // don't blank old optargs if our new argc lives in the old optargs.
  if (argv<toys.optargs || argv>toys.optargs+toys.optc) free(toys.optargs);
  memset(&toys, 0, offsetof(struct toy_context, rebound));
  if (toys.which) memset(&this, 0, sizeof(this));

  // Continue to portion of init needed by standalone commands
  toy_singleinit(which, argv);
}

// Runs an internal toybox command. Returns the exit value.
int toy_run(const struct toy_list *cmd, char *argv[])
{
    // Return if stack depth getting noticeable (proxy for leaked heap, etc).
    if (toys.stacktop && labs((char *)toys.stacktop-(char *)&cmd)>6000) {
      error_msg("stack full");
      return 1;
    }

    toy_init(cmd, argv);
    if (toys.which)
        toys.which->toy_main();

    return toys.exitval;
}
