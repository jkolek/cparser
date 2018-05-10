// Common stuff.
// Copyright (C) 2017, 2018  Jozef Kolek <jkolek@gmail.com>
//
// All rights reserved.
//
// See the LICENSE file for more details.

#ifndef COMMON_H
#define COMMON_H

#include <cstring>

#define IDLEN 256
#define MAXSTR 1024

#define FAIL -1

namespace cparser
{

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;

class CharCompare
{
public:
    bool operator()(const char *a, const char *b) const
    {
        return strcmp(a, b) < 0;
    }
};

} // namespace cparser

#endif
