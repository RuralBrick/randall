#include "rand64-mr.h"
#include <stdlib.h>
#include <time.h>

struct drand48_data buf = {0};

void
mrand_rand64_init(void) {
  long int seed = time(NULL);
  srand48_r(seed, &buf);
}
unsigned long long
mrand_rand64(void) {
  unsigned long long int x;
  long int xh, xl;
  mrand48_r(&buf, &xh);
  mrand48_r(&buf, &xl);
  x = xh;
  x <<= 32;
  x |= xl;
  return x;
}
void
mrand_rand64_fini(void) {}
