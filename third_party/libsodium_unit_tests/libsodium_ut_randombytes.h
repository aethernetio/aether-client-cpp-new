///
///\file libsodium_ut_randombytes.h
///\brief Unit tests for the sodium library randombytes functions.
///
///\details
///
///\author Dmitriy Kiryanov dmitriyk@aethernet.io
///\version 1.0.0
///\date  15.07.2024
///
#ifndef THIRD_PARTY_LIBSODIUM_UT_RANDOMBYTES_H
#define THIRD_PARTY_LIBSODIUM_UT_RANDOMBYTES_H

#include "cmptest.h"

namespace randombytes_sodium
{    
    #define TEST_NAME52 "randombytes"

    static unsigned char      x[65536];
    static unsigned long long freq[256];

    void _libsodium_ut_randombytes();
} // namespace generichash2

#endif /* THIRD_PARTY_LIBSODIUM_UT_RANDOMBYTES_H */
