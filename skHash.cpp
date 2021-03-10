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
#include "skHash.h"
#include "skString.h"


// magic numbers from http://www.isthe.com/chongo/tech/comp/fnv/
#define _SK_INITIAL_FNV 0x9E3779B1
#define _SK_INITIAL_FNV2 0x9E3779B9
#define _SK_MULTIPLE_FNV 0x1000193

#define _SK_TWHASH(key)  \
    key += ~(key << 15); \
    key ^= (key >> 10);  \
    key += (key << 3);   \
    key ^= (key >> 6);   \
    key += ~(key << 11); \
    key ^= (key >> 16);


SKhash skHash(const char* key)
{

    if (!key) return SK_NPOS;
	return skHash(key, skChar::length(key));
}


// Fowler/Noll/Vo (FNV) Hash
SKhash skHash(const char* key, SKsize len)
{

    if (!key || len == 0 || len == SK_NPOS)
        return SK_NPOS;

    SKsize i;
    SKhash hash = (SKhash)_SK_INITIAL_FNV;

    for (i = 0; key[i] && i < len; i++)
    {
        hash = hash ^ (key[i]);          // xor the low 8 bits
        hash = hash * _SK_MULTIPLE_FNV;  // multiply by the magic number
    }
    return hash;
}


SKhash skHash(const SKuint32& key)
{
    SKhash hash;

    hash = ((SKhash)(key)) * _SK_INITIAL_FNV;
    _SK_TWHASH(hash);
    return hash;
}

SKhash skHash(const void* key)
{
    SKhash hash;

    hash = (SKhash)((SKuintPtr)(key)) * _SK_INITIAL_FNV;
    _SK_TWHASH(hash);
    return hash;
}

SKhash skHash(const SKuint64& key)
{

    return skHash((void*)key);
}
