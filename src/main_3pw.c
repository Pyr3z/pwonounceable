/** @file   3pw.c
 *  @author [Tom Van Vleck](https://multicians.org/thvv/gpw.html)
 *  @author [Levi Perez](https://leviperez.dev/3pw)
 *  
 *  @brief  Generates passwords that are easier for a human to memorize.
 * 
 *  @details
 *    Adapted from Tom Van Vleck's (THVV) JavaScript version of `gpw`, this
 *    self-contained program generates a series of trigraphs — meaning 3-letter
 *    combinations which are statistically likely to occur near each other in a
 *    given dictionary (in this case, a standard GNU English dictionary).
 * 
 *    Levi's v2 Changelog:
 *    + Added PascalCase capitalization of variadic wordsize
 * 
 *    + Currently supports 3 positional arguments:
 *      arg1 - string length                  : int
 *      arg2 - number of strings to generate  : int
 *      arg3 - wordsize for capitalization    : int
 *           | pass 0 for no capitalization,
 *           | or 1 for all caps
 *      (pass '-' in any argument position to use the default value)
 * 
 *    History:
 *    - THVV  1994-06-01  Coded (v1)
 *    - THVV  1996-04-14  converted to Java
 *    - THVV  1997-07-30  fixed for Netscape 4.0
 *    - THVV  2009-11-27  ported to Javascript
 *    - Levi  2022-04-19  Re-coded in ANSI C (v2)
 * 
 *    For pre-me (Levi) history info, see
 *    <www.multicians.org/thvv/gpw.html>.
**/

/**
  @license MIT
  Permission is hereby granted, free of charge, to any person obtaining
  a copy of this software and associated documentation files (the
  "Software"), to deal in the Software without restriction, including
  without limitation the rights to use, copy, modify, merge, publish,
  distribute, sublicense, and/or sell copies of the Software, and to
  permit persons to whom the Software is furnished to do so, subject to
  the following conditions:
  
  The above copyright notice and this permission notice shall be included
  in all copies or substantial portions of the Software.
  
  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
  IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
  CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
  TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
  SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE. 
**/

/* ~ C INCLUDES */
#include <stdlib.h>   /* atoi, srand, rand, EXIT_SUCCESS */
#include <time.h>     /* time */
#include <stdio.h>    /* printf, puts */
#include <stdbool.h>  /* bool */

#include "../inc/errprintf.h" /* errprintf */
#include "../inc/strhash.h"   /* strhash   */


/* ~ CONSTANTS */
#define DEFAULT_PWL 9   /* default password length */
#define DEFAULT_QTY 8   /* default number of passwords to generate */
#define DEFAULT_CAP 6   /* default wordsize for PascalCase capitalization */

#ifndef DEFAULT_SEED
#define DEFAULT_SEED ((unsigned)time(0))
#endif

#define ARGI_PWL  1
#define ARGI_QTY  2
#define ARGI_CAP  3
#define ARGI_SEED 4


#define MAX_OUT_LEN 255         /* = maximum output string length. */
#define N_COMBS     125729      /* precalc'd value based on the combinatorix. */
#define CAP_FIRST   MAX_OUT_LEN /* wordsize to Capitalize only first letter. */
#define CAP_NONE    0           /* wordsize to Capitalize nothing. */


#undef  EXIT_SUCCESS /* to squelch pedantic gcc warnings; let's be explicit */
#undef  EXIT_FAILURE /* ^ */
#define EXIT_SUCCESS          0x00
#define EXIT_FAILURE          0x0F
#define EXIT_FAILURE_BADINPUT 0x1F
#define EXIT_FAILURE_SHITCODE 0xFF /* SHIT */


/* you might be able to play w/ this alphabet... /shrug */
static const char ABC[] = "abcdefghijklmnopqrstuvwxyz";
static const char CVS[] = "VCCCVCCCVCCCCCVCCCCCVCCC?C"; /* consonants/vowels */

#define N_ABC (int)(sizeof(ABC)-1)

/* raw data: use a preprocessor include to define data */
static const int TRIS[N_ABC][N_ABC][N_ABC]
#include "../inc/data-gpw.inl"


/* ~ FUNCTION PROTOTYPES */
static int  Do3pw(int pwl, char out[MAX_OUT_LEN+1]);
static int  PickTrigraph(int r, char out[3]);
static void Capitalize(char out[MAX_OUT_LEN+1], int wordsize);
static bool IsConsonant(char c);
/* ~ */


/* ~ PROGRAM ENTRY ~ begin my ANSI C interpretation of the gpw.js algo */
int main(int argc, const char* argv[])
{
  /* WARNING: CLI argument order is hard-coded! */
  /* TODO use getopt */

  int pwl, qty, cap;  /* input arguments, in order. */
  unsigned seed;      /* this one too. */

  pwl  = DEFAULT_PWL;
  qty  = DEFAULT_QTY;
  cap  = DEFAULT_CAP;
  seed = DEFAULT_SEED;
  switch (argc - 1)
  {
    /* yucky magic numbers & strings ... will clean l8ter */
    case ARGI_SEED:
      seed = (unsigned)atol(argv[ARGI_SEED]);
      if (!seed)
      {
        seed = strhash(argv[ARGI_SEED]);
        /*
        errprintf("DBG: string seed \"%s\" ~> %u\n", argv[ARGI_SEED], seed);
        */
      }
      /* intentional fall through */
    case ARGI_CAP:
      if (argv[ARGI_CAP][0] != '-' || argv[ARGI_CAP][1])
      {
        cap = atoi(argv[ARGI_CAP]);
      }
      /* ANOTHER intentional fall through */
    case ARGI_QTY:
      if (argv[ARGI_QTY][0] != '-' || argv[ARGI_QTY][1])
      {
        qty = atoi(argv[ARGI_QTY]);
      }
      /* weeeeeeeeeeeeeeeeeeeeee. */
    case ARGI_PWL:
      if (argv[ARGI_PWL][0] != '-' || argv[ARGI_PWL][1])
      {
        pwl = atoi(argv[ARGI_PWL]);
      }
      break;
  }

  if (pwl < 0)
  {
    errprintf("ERR arg%d: nonsensical negative value; "
              "pwl = %d\n", ARGI_PWL, pwl);
    exit(EXIT_FAILURE_BADINPUT);
  }
  else if (MAX_OUT_LEN < pwl)
  {
    errprintf("ERR arg%d: %d is too many characters; "
              "max = %d\n", ARGI_PWL, pwl, MAX_OUT_LEN);
    exit(EXIT_FAILURE_BADINPUT);
  }
  else if (pwl == 0)
  {
    /* early-out noop */
    exit(EXIT_SUCCESS);
  }

  if (qty < 0) /* == 0 is permitted as valid user input ~> just do no-op */
  {
    errprintf("ERR arg%d: %d is an invalid quantity of output strings; "
              "min = 0\n", ARGI_QTY, qty);
    exit(EXIT_FAILURE_BADINPUT);
  }

  /* done parsing input arguments. */

  srand(seed);
  
  while (qty --> 0)
  {
    char out[MAX_OUT_LEN+1] = { 0 };
    int  n;
    
    n = Do3pw(pwl, out);

    Capitalize(out, cap);
    puts(out);

    if (n != pwl)
    {
      errprintf("WRN: Bad output length? "
                "pwl = %d, n = %d, output = \"%s\"\n", pwl, n, out);
    }
  }

  exit(EXIT_SUCCESS);
}


/* ~ PRIVATE FUNCS --> EOF */

static int Do3pw(int pwl, char out[MAX_OUT_LEN+1])
{
  int r, n, sum;
  int i, j, k;

  /* Begin algo: */
  do
  {
    *((unsigned*)out) = 0x00000000; /* zeroes out first 4 bytes */

    r = rand() % N_COMBS;

    n = PickTrigraph(r, out);
    
    if (n < 3)
    {
      errprintf("ERR: EXIT_FAILURE_SHITCODE; PickTrigraph() returned %d. "
                "partial output = \"%s\"\n", n, out);
      exit(EXIT_FAILURE_SHITCODE);
    }
  } /* loop back if first trigraph is all consonants */
  while (IsConsonant(out[0]) && IsConsonant(out[1]) && IsConsonant(out[2]));
  
  if (n >= pwl) /* if smol pwl, need to return LESS than a trigraph */
  {
    /* note: leaves byte after the NUL unchanged in the case of pwl=1 */
    out[pwl] = 0;
    return pwl;
  }
  
  /* use combinatorix trigraph lookup to fuzzy select subsequent characters: */
  while (n < MAX_OUT_LEN && n < pwl)
  {
    sum = 0;
    i = out[n-2] - 'a';
    j = out[n-1] - 'a';

    for (k = 0; k < N_ABC; ++k)
    {
      sum += TRIS[i][j][k];
    }

    if (!sum)
    {
      /* no matching trigraphs at all ... */
      /* ... so just gonna improvise here ... */
      static const char VOWELS[] = "aeiouy";
      out[n++] = VOWELS[r % (sizeof(VOWELS)-1)];
      continue;
    }

    r = rand() % sum;
    sum = 0;

    for (k = 0; k < N_ABC; ++k)
    {
      sum += TRIS[i][j][k];
      if (sum > r)
      {
        out[n++] = ABC[k];
        break;
      }
    }
  } /* end while */

  return n;
}

static int PickTrigraph(int r, char out[3])
{
  int sum;
  int i, j, k;
  
  sum = 0;

  for (i = 0; i < N_ABC; ++i)
  {
    for (j = 0; j < N_ABC; ++j)
    {
      for (k = 0; k < N_ABC; ++k)
      {
        sum += TRIS[i][j][k];
        if (sum > r)
        {
          out[0] = ABC[i];
          out[1] = ABC[j];
          out[2] = ABC[k];
          return 3;
        }
      }
    }
  }

  return 0;
}

static void Capitalize(char out[MAX_OUT_LEN+1], int wordsize)
{
  int i;

  if (wordsize == CAP_NONE)
  {
    /* valid no-op -> leaves output lowercase */
    return;
  }

  if (wordsize < 0)
  {
    errprintf("ERR: EXIT_FAILURE_SHITCODE; wordsize = %d, "
              "partial output = \"%s\"\n", wordsize, out);
    exit(EXIT_FAILURE_SHITCODE);
  }

  /* do capitalize */
  for (i = 0; i < MAX_OUT_LEN+1 && out[i]; i += wordsize)
  {
    if (out[i] >= 'a' && out[i] <= 'z')
      out[i] -= ' ';
  }
}

static bool IsConsonant(char c) /* assumes c is lowercase! [a-z] */
{
  unsigned i = (c - 'a');
  return i < N_ABC && CVS[i] == 'C';
}
