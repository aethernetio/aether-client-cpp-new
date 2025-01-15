///
///\file libsodium_ut_auth7.h
///\brief Unit tests for the sodium library auth7 functions.
///
///\details
///
///\author Dmitriy Kiryanov dmitriyk@aethernet.io
///\version 1.0.0
///\date  11.07.2024
///
#ifndef THIRD_PARTY_LIBSODIUM_UT_AUTH7_H
#define THIRD_PARTY_LIBSODIUM_UT_AUTH7_H

#include "cmptest.h"

namespace auth7
{
    #define TEST_NAME13 "auth7"
    
    static unsigned char key[32];
    static unsigned char c[600];
    static unsigned char a[64];

    void _libsodium_ut_auth7();
} // namespace auth7

#endif /* THIRD_PARTY_LIBSODIUM_UT_AUTH7_H */
