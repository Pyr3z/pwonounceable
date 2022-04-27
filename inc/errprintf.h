/** @file   errprintf.h
 *  @author [Levi Perez](https://leviperez.dev/3pw)
 *  @date   2022-04-26
 *  
 *  @brief  Simple include implementing an easy printf interface for stderr.
**/

#pragma once
#ifndef ERRPRINTF_H_
#define ERRPRINTF_H_

#include <stdarg.h>
#include <stdio.h>

int errprintf(const char* fmt, ...)
{
  int ret;
  va_list vl;

  va_start(vl, fmt);
  ret = vfprintf(stderr, fmt, vl);
  va_end(vl);
  
  return ret;
}

#endif /* ERRPRINTF_H_ */
