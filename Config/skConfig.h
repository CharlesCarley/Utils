/*
-------------------------------------------------------------------------------

    Copyright (c) Charles Carley.

    Contributor(s): none yet.

-------------------------------------------------------------------------------
  This software is provided 'as-is', without any express or implied
  warranty. In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.
-------------------------------------------------------------------------------
*/
#ifndef _skConfig_h_
#define _skConfig_h_

#define Utils_USE_DEBUG_ASSERT 1
/* #undef Utils_USE_ITERATOR_DEBUG */
/* #undef Utils_USE_COMPILER_CHECKS */
#define Utils_NO_DEBUGGER 1

#ifdef Utils_USE_STD_STRING_FUNCS
#define SK_USE_STD_STRING_FUNCS 1
#endif

#ifdef Utils_NO_DEBUGGER
#define SK_NO_DEBUGGER 1
#endif

#ifdef Utils_USE_ITERATOR_DEBUG
#define SK_ITERATOR_DEBUG 1
#endif

#if defined(Utils_USE_DEBUG_ASSERT) && (defined(DEBUG) || defined(_DEBUG))
#include "Utils/skAssert.h"
#define SK_DEBUG 1
#define SK_ASSERT(x)                                            \
    {                                                           \
        if (!(x))                                               \
            skAssertTrap(#x, __FILE__, __LINE__, __FUNCTION__); \
    }
#define SK_ACCESSVIOLATION() \
    {                        \
        struct XYX           \
        {                    \
            int x;           \
        };                   \
        XYZ* x;              \
        x->x;                \
    }
#else
#define SK_ASSERT(x) ((void)0)
#define SK_ACCESSVIOLATION() \
    {                        \
        struct XYX           \
        {                    \
            int x;           \
        };                   \
        XYZ* x;              \
        x->x;                \
    }
#endif

#ifdef Utils_USE_COMPILER_CHECKS
#define SK_ASSERTCOMP(x, n) typedef bool x[(n) ? 1 : -1];
#else
#define SK_ASSERTCOMP(x, n)
#endif

#define SK_PLATFORM_WIN32 0
#define SK_PLATFORM_LINUX 2
#define SK_PLATFORM_APPLE 3
#define SK_PLATFORM_CYGWIN 4
#define SK_PLATFORM_ANDROID 5
#define SK_PLATFORM_EMSCRIPTEN 6

#if defined(Utils_BLD_ANDROID)
#define SK_PLATFORM SK_PLATFORM_ANDROID
#define SK_SUB_PLATFORM SK_PLATFORM_LINUX
#elif defined(__EMSCRIPTEN__)
#define SK_PLATFORM SK_PLATFORM_EMSCRIPTEN
#define SK_SUB_PLATFORM SK_PLATFORM_LINUX
#elif defined(_WIN32) || defined(__CYGWIN__)
#define SK_PLATFORM SK_PLATFORM_WIN32
#if defined(__CYGWIN__)
#define SK_SUB_PLATFORM SK_PLATFORM_CYGWIN
#else
#define SK_SUB_PLATFORM SK_PLATFORM_WIN32
#endif
#elif defined(__APPLE__)
#define SK_PLATFORM SK_PLATFORM_APPLE
#define SK_SUB_PLATFORM SK_PLATFORM_APPLE
#elif defined(__linux__) || defined(__unix__)
#define SK_PLATFORM SK_PLATFORM_LINUX
#define SK_SUB_PLATFORM SK_PLATFORM_LINUX
#else
#error Unknown platform
#endif

#define SK_COMPILER_MSVC 0
#define SK_COMPILER_GNU 1
#define SK_COMPILER_EMSCRIPTEN 2

#if defined(__EMSCRIPTEN__)
#define SK_COMPILER SK_COMPILER_EMSCRIPTEN
#elif defined(_MSC_VER)
#define SK_COMPILER SK_COMPILER_MSVC
#elif defined(__GNUC__)
#define SK_COMPILER SK_COMPILER_GNU
#else
#error unknown compiler
#endif

#define SK_ENDIAN_LITTLE 0
#define SK_ENDIAN_BIG 1

#if defined(__sgi) || defined(__sparc) ||     \
    defined(__sparc__) || defined(__PPC__) || \
    defined(__ppc__) || defined(__BIG_ENDIAN__)
#define SK_ENDIAN SK_ENDIAN_BIG
#else
#define SK_ENDIAN SK_ENDIAN_LITTLE
#endif

#if SK_ENDIAN == SK_ENDIAN_BIG
#define SK_ID(a, b, c, d) ((int)(a) << 24 | (int)(b) << 16 | (c) << 8 | (d))
#define SK_ID2(c, d) ((c) << 8 | (d))
#else
#define SK_ID(a, b, c, d) ((int)(d) << 24 | (int)(c) << 16 | (b) << 8 | (a))
#define SK_ID2(c, d) ((d) << 8 | (c))
#endif

#define SK_ARCH_32 0
#define SK_ARCH_64 1

#if defined(__x86_64__) || defined(_M_X64) ||       \
    defined(__powerpc64__) || defined(__alpha__) || \
    defined(__ia64__) || defined(__s390__) ||       \
    defined(__s390x__)
#define SK_ARCH SK_ARCH_64
#else
#define SK_ARCH SK_ARCH_32
#endif

#if SK_PLATFORM == SK_PLATFORM_WIN32
#if defined(__MINGW32__) || \
    defined(__CYGWIN__) ||  \
    (defined(_MSC_VER) && _MSC_VER < 1300)
#define SK_INLINE inline
#else
#define SK_INLINE __forceinline
#endif
#else
#define SK_INLINE inline
#endif

// Integer types
typedef long           SKlong;
typedef unsigned long  SKulong;
typedef int            SKint32;
typedef unsigned int   SKuint32;
typedef short          SKint16;
typedef unsigned short SKuint16;
typedef signed char    SKint8;
typedef unsigned char  SKuint8;
typedef unsigned char  SKubyte;
typedef char           SKbyte;

#if SK_COMPILER == SK_COMPILER_MSVC
typedef __int64          SKint64;
typedef unsigned __int64 SKuint64;
#else
typedef long long          SKint64;
typedef unsigned long long SKuint64;
#endif

#if SK_ARCH == SK_ARCH_64
typedef SKuint64 SKuintPtr;
typedef SKint64  SKintPtr;
#else
typedef SKuint32         SKuintPtr;
typedef SKint32          SKintPtr;
#endif

typedef SKuintPtr SKhash;
typedef SKuintPtr SKsize;

#define SK_MKNPOS(x) ((x)-1)
#define SK_MKMX(x) (SK_MKNPOS(x) - 1)
#define SK_MKMXH(x) (SK_MKNPOS(x) >> 1)

const SKsize   SK_NPOS    = SK_MKNPOS(SKsize);
const SKsize   SK_NPOSH   = SK_MKMXH(SKsize);
const SKsize   SK_MAX     = SK_MKMX(SKsize);
const SKuint16 SK_NPOS16  = SK_MKNPOS(SKuint16);
const SKuint16 SK_MAX16   = SK_MKMX(SKuint16);
const SKuint16 SK_NPOS16H = SK_MKMXH(SKuint16);
const SKuint32 SK_NPOS32  = SK_MKNPOS(SKuint32);
const SKuint32 SK_MAX32   = SK_MKMX(SKuint32);
const SKuint32 SK_NPOS32H = SK_MKMXH(SKuint32);

#define SKInitalCap 8

#if SK_ITERATOR_DEBUG == 1
#define SK_ITER_DEBUG(x) SK_ASSERT(x)
#else
#define SK_ITER_DEBUG(x)
#endif

#define SK_MAKE_HANDLE(name) \
    typedef struct name##_t  \
    {                        \
        int unused;          \
    } * name

#ifdef __cplusplus
#define SK_API extern "C"
#define SK_ST_C \
    extern "C"  \
    {
#define SK_EN_C }
#else
#define SK_API extern
#define SK_ST_C
#define SK_EN_C
#endif

#define SK_CHECK(X, VALUE) \
    {                      \
        if (!(X))          \
            return VALUE;  \
    }
    
#define SK_VOID
#define SK_SBUF_SIZE 1024

#endif  //_skConfig_h_
