#ifndef OPTIONS_H
#define OPTIONS_H

#include <stdbool.h>

struct optStruct {
  long long nbytes;
  int iOpt;         // 0:rdrand, 1:mrand48_r, 2:/F
  char *input;      // F
  int oOpt;         // 0:stdio, 1:N
  long long output; // N
};

bool parseLongLong(char *str, long long *ll);
int parseInput(char *input);
int parseOutput(char *output, long long *n);
bool parseOptions(int argc, char **argv, struct optStruct *options);

#endif
