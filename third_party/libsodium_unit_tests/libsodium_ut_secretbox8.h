///
///\file libsodium_ut_secretbox8.h
///\brief Unit tests for the sodium library secretbox8 functions.
///
///\details
///
///\author Dmitriy Kiryanov dmitriyk@aethernet.io
///\version 1.0.0
///\date  15.07.2024
///
#ifndef THIRD_PARTY_LIBSODIUM_UT_SECRETBOX8_H
#define THIRD_PARTY_LIBSODIUM_UT_SECRETBOX8_H

#include "cmptest.h"

namespace secretbox8
{    
    #define TEST_NAME66 "secretbox8"

    static unsigned char k[crypto_secretbox_KEYBYTES];
    static unsigned char n[crypto_secretbox_NONCEBYTES];
    static unsigned char m[10000];
    static unsigned char c[10000];
    static unsigned char m2[10000];

    void _libsodium_ut_secretbox8();
} // namespace secretbox8

#endif /* THIRD_PARTY_LIBSODIUM_UT_SECRETBOX8_H */
