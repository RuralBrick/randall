#include "options.h"
#include <stdbool.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>

bool
parseNBytes(long long *nbytes, int argc, char **argv) {
  if (argc == 2) {
    char *endptr;
    errno = 0;
    *nbytes = strtoll(argv[1], &endptr, 10);
    if (errno) {
      perror(argv[1]);
      return false;
    }
    return !*endptr && 0 <= *nbytes;
  }
  return false;
}
