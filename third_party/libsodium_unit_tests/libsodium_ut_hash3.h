///
///\file libsodium_ut_hash3.h
///\brief Unit tests for the sodium library hash3 functions.
///
///\details
///
///\author Dmitriy Kiryanov dmitriyk@aethernet.io
///\version 1.0.0
///\date  15.07.2024
///
#ifndef THIRD_PARTY_LIBSODIUM_UT_HASH3_H
#define THIRD_PARTY_LIBSODIUM_UT_HASH3_H

#include "cmptest.h"

namespace hash3
{    
    #define TEST_NAME38 "hash3"

    static unsigned char x[] = "testing\n";
    static unsigned char h[crypto_hash_BYTES];

    void _libsodium_ut_hash3();
} // namespace hash3

#endif /* THIRD_PARTY_LIBSODIUM_UT_HASH3_H */
