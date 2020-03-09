#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dlist.h"

typedef struct _int_list
{
  dlist_t _;
  int data;
} integer;

void *
integer_new (int data)
{
  integer *new_int = calloc(1, sizeof(integer));
  if (!new_int)
    return NULL;

  new_int->data = data;

  return new_int;
}

DLIST_HEAD(header);

int
main (int argc, char *argv[])
{
  integer *new;

  new = integer_new (10);
  if (!new)
    return -1;
  dlist_append (&header, GET_DLIST (new));

  new = integer_new (12);
  if (!new)
    return -1;
  dlist_append (&header, GET_DLIST (new));

  new = integer_new (14);
  if (!new)
    return -1;
  dlist_append (&header, GET_DLIST (new));

  new = integer_new (16);
  if (!new)
    return -1;
  dlist_append (&header, GET_DLIST (new));

  new = integer_new (18);
  if (!new)
    return -1;
  dlist_append (&header, GET_DLIST (new));

  new = integer_new (20);
  if (!new)
    return -1;
  dlist_append (&header, GET_DLIST (new));

  dlist_foreach_safe (&header) {
    integer *v = (integer *)__ptr;

    printf("%d ", v->data);
    dlist_remove (GET_DLIST (v));
    if (v)
      free (v);
  }
  putchar('\n');

  return 0;
}
