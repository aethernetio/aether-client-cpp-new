///
///\file libsodium_ut_auth5.h
///\brief Unit tests for the sodium library auth5 functions.
///
///\details
///
///\author Dmitriy Kiryanov dmitriyk@aethernet.io
///\version 1.0.0
///\date  11.07.2024
///
#ifndef THIRD_PARTY_LIBSODIUM_UT_AUTH5_H
#define THIRD_PARTY_LIBSODIUM_UT_AUTH5_H

#include "cmptest.h"

namespace auth5
{
    #define TEST_NAME11 "auth5"
    
    static unsigned char key[32];
    static unsigned char c[1000];
    static unsigned char a[32];

    void _libsodium_ut_auth5();
} // namespace auth5

#endif /* THIRD_PARTY_LIBSODIUM_UT_AUTH5_H */
