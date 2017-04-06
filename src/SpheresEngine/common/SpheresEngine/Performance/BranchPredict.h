#pragma once

/**
 * Defines for branch prediction instructions which won't block non-GCC compilers
 */

#ifdef __GNUC__ 
#define GCC_LIKELY(x)    __builtin_expect ( x, 1)
#define GCC_UNLIKELY(x)  __builtin_expect ( x, 0)
#else
#define GCC_LIKELY(x)    ( x )
#define GCC_UNLIKELY(x)  ( x )
#endif
