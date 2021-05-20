/* Generate N bytes of random output.  */

/* When generating output this program uses the x86-64 RDRAND
   instruction if available to generate random numbers, falling back
   on /dev/random and stdio otherwise.

   This program is not portable.  Compile it with gcc -mrdrnd for a
   x86-64 machine.

   Copyright 2015, 2017, 2020 Paul Eggert

   This program is free software: you can redistribute it and/or
   modify it under the terms of the GNU General Public License as
   published by the Free Software Foundation, either version 3 of the
   License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful, but
   WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

#include "options.h"
#include "output.h"
#include "rand64-hw.h"
#include "rand64-sw.h"
#include "rand64-mr.h"
#include <stdbool.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <limits.h>
#include <unistd.h>

/* Main program, which outputs N bytes of random data.  */
int
main (int argc, char **argv)
{
  struct optStruct options;
  bool valid = parseOptions(argc, argv, &options);
  if (!valid)
    {
      fprintf (stderr, "%s: usage: %s [-i INPUT] [-o OUTPUT] NBYTES\n", argv[0], argv[0]);
      return 1;
    }

  long long nbytes = options.nbytes;
  int iOpt = options.iOpt;
  int oOpt = options.oOpt;
  
  /* If there's no work to do, don't worry about which library to use.  */
  if (nbytes == 0)
    return 0;

  /* Now that we know we have work to do, arrange to use the
     appropriate library.  */
  void (*initialize) (void);
  unsigned long long (*rand64) (void);
  void (*finalize) (void);

  switch (iOpt) {
  case 0:
    if (rdrand_supported()) {
      initialize = hardware_rand64_init;
      rand64 = hardware_rand64;
      finalize = hardware_rand64_fini;
      break;
    }
    fprintf(stderr, "rdrand not available");
    return 1;
  case 1:
    initialize = mrand_rand64_init;
    rand64 = mrand_rand64;
    finalize = mrand_rand64_fini;
    break;
  case 2:
    initialize = software_rand64_init;
    rand64 = software_rand64;
    finalize = software_rand64_fini;
    setFile(options.input);
    break;
  default:
    fprintf(stderr, "No rand generator selected");
    return 1;
  }

  initialize ();
  int wordsize = sizeof rand64 ();
  int output_errno = 0;

  if (oOpt == 0) {
    do
      {
	unsigned long long x = rand64 ();
	int outbytes = nbytes < wordsize ? nbytes : wordsize;
	if (!writebytes (x, outbytes))
	  {
	    output_errno = errno;
	    break;
	  }
	nbytes -= outbytes;
      }
    while (0 < nbytes);
  }
  else if (oOpt == 1) {
    int bufferSize = options.output;
    char *buffer = malloc(bufferSize);
    int index = 0;
    
    while (nbytes > 0) {
      unsigned long long x = rand64();

      if (nbytes - bufferSize < 0)
	bufferSize = nbytes;

      while (x > 0 && index < bufferSize) {
	buffer[index] = x;
	x >>= CHAR_BIT;
	index++;
      }

      if (index == bufferSize) {
	nbytes -= write(1, buffer, bufferSize);
	index = 0;
      }
    }
    
    free(buffer);
  }
  else {
    fprintf(stderr, "No output selected");
    return 1;
  }
  
  if (fclose (stdout) != 0)
    output_errno = errno;

  if (output_errno)
    {
      errno = output_errno;
      perror ("output");
    }

  finalize ();
  return !!output_errno;
}
