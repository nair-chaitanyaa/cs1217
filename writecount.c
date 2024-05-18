#include "types.h"
#include "stat.h"
#include "user.h"

int
main(int argc, char *argv[])
{
  int count = writecount();
  printf(1, "Number of write system calls: %d\n", count);
  exit();
}