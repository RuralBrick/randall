#include "options.h"
#include <stdbool.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

bool
parseLongLong(char *str, long long *ll) {
  char *endptr;
  errno = 0;
  *ll = strtoll(str, &endptr, 10);
  if (errno) {
    perror(str);
    return false;
  }
  return !*endptr && 0 <= *ll;
}

int
parseInput(char *input) {
  if (strcmp(input, "rdrand") == 0)
    return 0;
  if (strcmp(input, "mrand48_r") == 0)
    return 1;
  if (input[0] == '/')
    return 2;
  return -1;
}

int parseOutput(char *output, long long *n) {
  if (strcmp(output, "stdio") == 0)
    return 0;
  if (parseLongLong(output, n))
    return 1;
  return -1;
}

bool
parseOptions(int argc, char **argv, struct optStruct *options) {
  int opt, nNonOptArgs;

  options->iOpt = 0;
  options->oOpt = 0;
  
  while ((opt = getopt(argc, argv, "i:o:")) != -1) {
    switch (opt) {
    case 'i':
      if ((options->iOpt = parseInput(optarg)) == -1)
	return false;
      options->input = optarg;
      break;
    case 'o':
      if ((options->oOpt = parseOutput(optarg, &options->output)) == -1)
	return false;
      break;
    default: return false;
    }
  }

  nNonOptArgs = argc - optind;
  if (nNonOptArgs == 1 && parseLongLong(argv[optind], &options->nbytes))
    return true;
  return false;
}
