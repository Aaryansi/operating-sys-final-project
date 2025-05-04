#include "kernel/types.h"
#include "user/user.h"

int
main(int argc, char **argv)
{
  char *p = (char *)0xdeadbeef;

  /* Try to access an invalid pointer and check what happens. */
  *p = 0xff;
  printf("The ptr %p points to %c", p, *p);

  exit(0);
}
