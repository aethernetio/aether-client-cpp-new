///
///\file libsodium_ut_core1.h
///\brief Unit tests for the sodium library core1 functions.
///
///\details
///
///\author Dmitriy Kiryanov dmitriyk@aethernet.io
///\version 1.0.0
///\date  15.07.2024
///
#ifndef THIRD_PARTY_LIBSODIUM_UT_CORE1_H
#define THIRD_PARTY_LIBSODIUM_UT_CORE1_H

#include "cmptest.h"

namespace core1
{
    #define TEST_NAME27 "core1"

    static unsigned char shared[32] = { 0x4a, 0x5d, 0x9d, 0x5b, 0xa4, 0xce, 0x2d,
                                        0xe1, 0x72, 0x8e, 0x3b, 0xf4, 0x80, 0x35,
                                        0x0f, 0x25, 0xe0, 0x7e, 0x21, 0xc9, 0x47,
                                        0xd1, 0x9e, 0x33, 0x76, 0xf0, 0x9b, 0x3c,
                                        0x1e, 0x16, 0x17, 0x42 };

    static unsigned char zero[32];

    static unsigned char c[16] = { 0x65, 0x78, 0x70, 0x61, 0x6e, 0x64, 0x20, 0x33,
                                   0x32, 0x2d, 0x62, 0x79, 0x74, 0x65, 0x20, 0x6b };

    static unsigned char firstkey[32];

    void _libsodium_ut_core1();
} // namespace core1

#endif /* THIRD_PARTY_LIBSODIUM_UT_CORE1_H */
