///
///\file libsodium_ut_core3.h
///\brief Unit tests for the sodium library core3 functions.
///
///\details
///
///\author Dmitriy Kiryanov dmitriyk@aethernet.io
///\version 1.0.0
///\date  15.07.2024
///
#ifndef THIRD_PARTY_LIBSODIUM_UT_CORE3_H
#define THIRD_PARTY_LIBSODIUM_UT_CORE3_H

#include "cmptest.h"

namespace core3
{    
    #define TEST_NAME29 "core3"

    static unsigned char SECONDKEY[32] = { 0xdc, 0x90, 0x8d, 0xda, 0x0b, 0x93, 0x44,
                                           0xa9, 0x53, 0x62, 0x9b, 0x73, 0x38, 0x20,
                                           0x77, 0x88, 0x80, 0xf3, 0xce, 0xb4, 0x21,
                                           0xbb, 0x61, 0xb9, 0x1c, 0xbd, 0x4c, 0x3e,
                                           0x66, 0x25, 0x6c, 0xe4 };

    static unsigned char NONCESUFFIX[8] = { 0x82, 0x19, 0xe0, 0x03,
                                            0x6b, 0x7a, 0x0b, 0x37 };

    static unsigned char C[16] = { 0x65, 0x78, 0x70, 0x61, 0x6e, 0x64, 0x20, 0x33,
                                   0x32, 0x2d, 0x62, 0x79, 0x74, 0x65, 0x20, 0x6b };

    void _libsodium_ut_core3();
} // namespace core3

#endif /* THIRD_PARTY_LIBSODIUM_UT_CORE3_H */
