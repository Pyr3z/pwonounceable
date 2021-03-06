/** @file   strhash.h
 *  @author [Levi Perez](https://leviperez.dev)
 *  @date   2022-04-26
 *  
 *  @brief  Simple include implementing a dumb string hash algorithm unworthy
 *          of special name or mention.
**/

#pragma once
#ifndef STRHASH_H_
#define STRHASH_H_

unsigned strnhash(const char* str, int n)
{
  unsigned hash = 0xDEADBEEF * n;
    /* lol i'm too lazy to check if DEADBEEF is even prime (for now) */

  while (n --> 0 && str[n])
  {
    hash = ((unsigned)str[n]) ^ (hash + ((hash << 5) | (hash >> 27)));
  }

  return hash;
}

unsigned strhash(const char* str)
{
  /* implement strlen() to avoid including ALL of <string.h> */
  int n = 0;
  const char* walk = str;

  while (*walk++ && ++n) ;

  return strnhash(str, n);
}

#endif /* STRHASH_H_ */
