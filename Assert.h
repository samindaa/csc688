/*
 * Assert.h
 *
 *  Created on: Mar 25, 2014
 *      Author: sam
 */

#ifndef CUSTOM_ASSERT_H_
#define CUSTOM_ASSERT_H_

//*****************************************************************************
//
// The ASSERT macro, which does the actual assertion checking.  Typically, this
// will be for procedure arguments.
//
//*****************************************************************************
#if defined(ENERGIA)
#include "Energia.h"
#else
#include <cstdio>
#include <cstdlib>
#endif

#if defined(NDEBUG)
#define ASSERT(expr)   ((void)0)
#else
#define ASSERT(expr) do                                                       \
                     {                                                        \
                         if(!(expr))                                          \
                         {                                                    \
                             __ASSERT(__func__, __FILE__, __LINE__, #expr);   \
                         }                                                    \
                     }                                                        \
                     while(0)
#endif /* NDEBUG */

#if defined(ENERGIA)
static inline void __ASSERT(const char *__func, const char *__file, int __lineno, const char *__sexp)
{
  do
  {
    // TODO: write the error to serial port
  }
  while(0);
}
#else

static inline void __ASSERT(const char *__func, const char *__file, int __lineno,
    const char *__sexp)
{
  printf("Failed assertion! __func=%s __file=%s  __lineno=%u __sexp=%s \n", __func, __file,
      __lineno, __sexp);
  abort();
}
#endif

#endif /* CUSTOM_ASSERT_H_ */
