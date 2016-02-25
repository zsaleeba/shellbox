/* getmountlist.c - Get a linked list of mount points, with stat information.
 *
 * Copyright 2006 Rob Landley <rob@landley.net>
 */

#include "toys.h"
#include "toylib/lib.h"

// Realloc *old with oldstring,newstring

void comma_collate(char **old, char *new)
{
  char *temp, *atold = *old;

  // Only add a comma if old string didn't end with one
  if (atold && *atold) {
    char *comma = ",";

    if (atold[strlen(atold)-1] == ',') comma = "";
    temp = xmprintf("%s%s%s", atold, comma, new);
  } else temp = xstrdup(new);
  free (atold);
  *old = temp;
}

// iterate through strings in a comma separated list.
// returns start of next entry or NULL if none
// sets *len to length of entry (not including comma)
// advances *list to start of next entry
char *comma_iterate(char **list, int *len)
{
  char *start = *list, *end;

  if (!*list || !**list) return 0;

  if (!(end = strchr(*list, ','))) {
    *len = strlen(*list);
    *list = 0;
  } else *list += (*len = end-start)+1;

  return start;
}

// check all instances of opt and "no"opt in optlist, return true if opt
// found and last instance wasn't no. If clean, remove each instance from list.
int comma_scan(char *optlist, char *opt, int clean)
{
  int optlen = strlen(opt), len, no, got = 0;

  if (optlist) for (;;) {
    char *s = comma_iterate(&optlist, &len);

    if (!s) break;
    no = 2*(*s == 'n' && s[1] == 'o');
    if (optlen == len-no && !strncmp(opt, s+no, optlen)) {
      got = !no;
      if (clean && optlist) memmove(s, optlist, strlen(optlist)+1);
    }
  }

  return got;
}

// return true if all scanlist options enabled in optlist
int comma_scanall(char *optlist, char *scanlist)
{
  int i = 1;

  while (scanlist && *scanlist) {
    char *opt = comma_iterate(&scanlist, &i), *s = xstrndup(opt, i);

    i = comma_scan(optlist, s, 0);
    free(s);
    if (!i) break;
  }

  return i;
}
