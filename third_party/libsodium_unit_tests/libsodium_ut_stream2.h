///
///\file libsodium_ut_stream2.h
///\brief Unit tests for the sodium library stream2 functions.
///
///\details
///
///\author Dmitriy Kiryanov dmitriyk@aethernet.io
///\version 1.0.0
///\date  15.07.2024
///
#ifndef THIRD_PARTY_LIBSODIUM_UT_STREAM2_H
#define THIRD_PARTY_LIBSODIUM_UT_STREAM2_H

#include "cmptest.h"

namespace stream2
{    
    #define TEST_NAME77 "stream2"

    static const unsigned char secondkey[32] = {
        0xdc, 0x90, 0x8d, 0xda, 0x0b, 0x93, 0x44,
        0xa9, 0x53, 0x62, 0x9b, 0x73, 0x38, 0x20,
        0x77, 0x88, 0x80, 0xf3, 0xce, 0xb4, 0x21,
        0xbb, 0x61, 0xb9, 0x1c, 0xbd, 0x4c, 0x3e,
        0x66, 0x25, 0x6c, 0xe4
    };

    static const unsigned char noncesuffix[8] = {
        0x82, 0x19, 0xe0, 0x03, 0x6b, 0x7a, 0x0b, 0x37
    };

    void _libsodium_ut_stream2();
} // namespace stream2

#endif /* THIRD_PARTY_LIBSODIUM_UT_STREAM2_H */
