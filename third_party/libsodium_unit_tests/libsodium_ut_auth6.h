///
///\file libsodium_ut_auth6.h
///\brief Unit tests for the sodium library auth6 functions.
///
///\details
///
///\author Dmitriy Kiryanov dmitriyk@aethernet.io
///\version 1.0.0
///\date  11.07.2024
///
#ifndef THIRD_PARTY_LIBSODIUM_UT_AUTH6_H
#define THIRD_PARTY_LIBSODIUM_UT_AUTH6_H

#include "cmptest.h"

namespace auth6
{
    #define TEST_NAME12 "auth6"
    
    /* "Test Case 2" from RFC 4231 */
    static unsigned char key[32] = "Jefe";
    static unsigned char c[]     = "what do ya want for nothing?";

    static unsigned char a[64];

    void _libsodium_ut_auth6();
} // namespace auth6

#endif /* THIRD_PARTY_LIBSODIUM_UT_AUTH6_H */
