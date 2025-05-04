#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char **argv) {
  unsigned int i;
  int *p = malloc(10 *sizeof(int));

  for(i = 0; i < 10; i++)
    p[i] = i;

  for(i = 9; i >= 0; i--)
    printf("index: %d, value: %d\n", i, p[i]);

  free(p);

  exit(0);
}
