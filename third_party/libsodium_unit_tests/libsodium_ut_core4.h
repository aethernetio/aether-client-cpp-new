///
///\file libsodium_ut_core4.h
///\brief Unit tests for the sodium library core4 functions.
///
///\details
///
///\author Dmitriy Kiryanov dmitriyk@aethernet.io
///\version 1.0.0
///\date  15.07.2024
///
#ifndef THIRD_PARTY_LIBSODIUM_UT_CORE4_H
#define THIRD_PARTY_LIBSODIUM_UT_CORE4_H

#include "cmptest.h"

namespace core4
{    
    #define TEST_NAME30 "core4"

    static unsigned char k[32] = { 1,   2,   3,   4,   5,   6,   7,   8,
                                   9,   10,  11,  12,  13,  14,  15,  16,
                                   201, 202, 203, 204, 205, 206, 207, 208,
                                   209, 210, 211, 212, 213, 214, 215, 216 };

    static unsigned char in[16] = { 101, 102, 103, 104, 105, 106, 107, 108,
                                    109, 110, 111, 112, 113, 114, 115, 116 };

    static unsigned char c[16] = { 101, 120, 112, 97,  110, 100, 32, 51,
                                   50,  45,  98,  121, 116, 101, 32, 107 };

    static unsigned char out[64];

    void _libsodium_ut_core4();
} // namespace core4

#endif /* THIRD_PARTY_LIBSODIUM_UT_CORE4_H */
