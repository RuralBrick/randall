#include "rand64-sw.h"
#include <stdio.h>

void
software_rand64_init (void)
{
  urandstream = fopen ("/dev/random", "r");
  if (! urandstream)
    abort ();
}
unsigned long long
software_rand64 (void)
{
  unsigned long long int x;
  if (fread (&x, sizeof x, 1, urandstream) != 1)
    abort ();
  return x;
}
void
software_rand64_fini (void)
{
  fclose (urandstream);
}
