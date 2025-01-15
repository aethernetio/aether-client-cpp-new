///
///\file libsodium_ut_secretbox2.h
///\brief Unit tests for the sodium library secretbox2 functions.
///
///\details
///
///\author Dmitriy Kiryanov dmitriyk@aethernet.io
///\version 1.0.0
///\date  15.07.2024
///
#ifndef THIRD_PARTY_LIBSODIUM_UT_SECRETBOX2_H
#define THIRD_PARTY_LIBSODIUM_UT_SECRETBOX2_H

#include "cmptest.h"

namespace secretbox2
{    
    #define TEST_NAME64 "secretbox2"

    static unsigned char firstkey[32] = { 0x1b, 0x27, 0x55, 0x64, 0x73, 0xe9, 0x85,
                                          0xd4, 0x62, 0xcd, 0x51, 0x19, 0x7a, 0x9a,
                                          0x46, 0xc7, 0x60, 0x09, 0x54, 0x9e, 0xac,
                                          0x64, 0x74, 0xf2, 0x06, 0xc4, 0xee, 0x08,
                                          0x44, 0xf6, 0x83, 0x89 };

    static unsigned char nonce[24] = { 0x69, 0x69, 0x6e, 0xe9, 0x55, 0xb6,
                                       0x2b, 0x73, 0xcd, 0x62, 0xbd, 0xa8,
                                       0x75, 0xfc, 0x73, 0xd6, 0x82, 0x19,
                                       0xe0, 0x03, 0x6b, 0x7a, 0x0b, 0x37 };

    /* API requires first 16 bytes to be 0 */
    static unsigned char c[163] = {
        0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
        0,    0,    0,    0,    0xf3, 0xff, 0xc7, 0x70, 0x3f, 0x94, 0x00, 0xe5,
        0x2a, 0x7d, 0xfb, 0x4b, 0x3d, 0x33, 0x05, 0xd9, 0x8e, 0x99, 0x3b, 0x9f,
        0x48, 0x68, 0x12, 0x73, 0xc2, 0x96, 0x50, 0xba, 0x32, 0xfc, 0x76, 0xce,
        0x48, 0x33, 0x2e, 0xa7, 0x16, 0x4d, 0x96, 0xa4, 0x47, 0x6f, 0xb8, 0xc5,
        0x31, 0xa1, 0x18, 0x6a, 0xc0, 0xdf, 0xc1, 0x7c, 0x98, 0xdc, 0xe8, 0x7b,
        0x4d, 0xa7, 0xf0, 0x11, 0xec, 0x48, 0xc9, 0x72, 0x71, 0xd2, 0xc2, 0x0f,
        0x9b, 0x92, 0x8f, 0xe2, 0x27, 0x0d, 0x6f, 0xb8, 0x63, 0xd5, 0x17, 0x38,
        0xb4, 0x8e, 0xee, 0xe3, 0x14, 0xa7, 0xcc, 0x8a, 0xb9, 0x32, 0x16, 0x45,
        0x48, 0xe5, 0x26, 0xae, 0x90, 0x22, 0x43, 0x68, 0x51, 0x7a, 0xcf, 0xea,
        0xbd, 0x6b, 0xb3, 0x73, 0x2b, 0xc0, 0xe9, 0xda, 0x99, 0x83, 0x2b, 0x61,
        0xca, 0x01, 0xb6, 0xde, 0x56, 0x24, 0x4a, 0x9e, 0x88, 0xd5, 0xf9, 0xb3,
        0x79, 0x73, 0xf6, 0x22, 0xa4, 0x3d, 0x14, 0xa6, 0x59, 0x9b, 0x1f, 0x65,
        0x4c, 0xb4, 0x5a, 0x74, 0xe3, 0x55, 0xa5
    };

    static unsigned char m[163];

    void _libsodium_ut_secretbox2();
} // namespace secretbox2

#endif /* THIRD_PARTY_LIBSODIUM_UT_SECRETBOX2_H */
