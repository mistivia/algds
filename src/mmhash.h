// Copyright (C) 2023 Arcane Lactiflora <arcanelactiflora@outlook.com>
// Licensed under GPLv3. See LICENSE for details.

#ifndef ALGDS_HASH_H_
#define ALGDS_HASH_H_

#include <stdint.h>

uint64_t mmhash(const void *key, int len, uint64_t seed);

#endif
