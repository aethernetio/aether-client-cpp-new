///
///\file libsodium_ut_auth.h
///\brief Unit tests for the sodium library auth functions.
///
///\details
///
///\author Dmitriy Kiryanov dmitriyk@aethernet.io
///\version 1.0.0
///\date  11.07.2024
///
#ifndef THIRD_PARTY_LIBSODIUM_UT_AUTH_H
#define THIRD_PARTY_LIBSODIUM_UT_AUTH_H

#include "cmptest.h"

namespace auth
{
    #define TEST_NAME8 "auth"

    /* "Test Case 2" from RFC 4231 */
    static unsigned char key[32] = "Jefe";
    static unsigned char c[]     = "what do ya want for nothing?";

    /* Hacker manifesto */
    static unsigned char key2[] =
        "Another one got caught today, it's all over the papers. \"Teenager "
        "Arrested in Computer Crime Scandal\", \"Hacker Arrested after Bank "
        "Tampering\"... Damn kids. They're all alike.";

    static unsigned char a[crypto_auth_BYTES];
    static unsigned char a2[crypto_auth_hmacsha512_BYTES];
    static unsigned char a3[crypto_auth_hmacsha512_BYTES];

    void _libsodium_ut_auth();
} // namespace auth

#endif /* THIRD_PARTY_LIBSODIUM_UT_AUTH_H */
