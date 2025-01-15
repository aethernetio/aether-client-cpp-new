///
///\file libsodium_ut_scalarmult2.h
///\brief Unit tests for the sodium library scalarmult2 functions.
///
///\details
///
///\author Dmitriy Kiryanov dmitriyk@aethernet.io
///\version 1.0.0
///\date  15.07.2024
///
#ifndef THIRD_PARTY_LIBSODIUM_UT_SCALARMULT2_H
#define THIRD_PARTY_LIBSODIUM_UT_SCALARMULT2_H

#include "cmptest.h"

namespace scalarmult2
{    
    #define TEST_NAME56 "scalarmult2"

    static unsigned char bobsk[32] = { 0x5d, 0xab, 0x08, 0x7e, 0x62, 0x4a, 0x8a,
                                       0x4b, 0x79, 0xe1, 0x7f, 0x8b, 0x83, 0x80,
                                       0x0e, 0xe6, 0x6f, 0x3b, 0xb1, 0x29, 0x26,
                                       0x18, 0xb6, 0xfd, 0x1c, 0x2f, 0x8b, 0x27,
                                       0xff, 0x88, 0xe0, 0xeb };
    
    void _libsodium_ut_scalarmult2();
} // namespace generichash2

#endif /* THIRD_PARTY_LIBSODIUM_UT_SCALARMULT2_H */
