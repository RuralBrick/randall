#include "options.h"
#include <stdbool.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

bool
parseOptions(int argc, char **argv, long long *nbytes, char **input, char **output) {
  int nNonOptArgs;
  int opt;
  
  while ((opt = getopt(argc, argv, "i:o:")) != -1) {
    switch (opt) {
    case 'i': *input = optarg; break;
    case 'o': *output = optarg; break;
    default: return false;
    }
  }

  nNonOptArgs = argc - optind;
  if (nNonOptArgs == 0)
    return true;
  if (nNonOptArgs == 1) {
    char *endptr;
    errno = 0;
    *nbytes = strtoll(argv[optind], &endptr, 10);
    if (errno) {
      perror(argv[optind]);
      return false;
    }
    return !*endptr && 0 <= *nbytes;
  }
  return false;
}
