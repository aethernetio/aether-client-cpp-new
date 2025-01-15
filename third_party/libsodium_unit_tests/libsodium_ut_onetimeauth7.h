///
///\file libsodium_ut_onetimeauth7.h
///\brief Unit tests for the sodium library onetimeauth7 functions.
///
///\details
///
///\author Dmitriy Kiryanov dmitriyk@aethernet.io
///\version 1.0.0
///\date  15.07.2024
///
#ifndef THIRD_PARTY_LIBSODIUM_UT_ONETIMEAUTH7_H
#define THIRD_PARTY_LIBSODIUM_UT_ONETIMEAUTH7_H

#include "cmptest.h"

namespace onetimeauth7
{    
    #define TEST_NAME47 "onetimeauth7"

    static unsigned char key[32];
    static unsigned char c[1000];
    static unsigned char a[16];

    void _libsodium_ut_onetimeauth7();
} // namespace onetimeauth7

#endif /* THIRD_PARTY_LIBSODIUM_UT_ONETIMEAUTH7_H */
