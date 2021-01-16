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
#ifndef _skPlatformHeaders_h_
#define _skPlatformHeaders_h_

#include "Config/skConfig.h"

#if SK_PLATFORM == SK_PLATFORM_WIN32
#if SK_COMPILER == SK_COMPILER_MSVC
#pragma warning(disable : 4996)
#define _WIN32_WINNT 0x403
#endif
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN 1
#endif
#include <io.h>
#include <Windows.h>

// Used for storing pointers in handles.
#ifdef _WIN64
#define GWL_USER GWLP_USERDATA
#else
#define GWL_USER GWL_USERDATA
#endif

#else
#define __STDC_WANT_LIB_EXT1__ 1
#endif

#include <memory.h>
#include <cstdarg>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#if SK_COMPILER == SK_COMPILER_MSVC
#define skp_printf(ptr, size, fmt, lst) _vsnprintf_s(ptr, size, _TRUNCATE, fmt, lst)
#else
#define skp_printf vsnprintf
#endif

#endif  //_skPlatformHeaders_h_
