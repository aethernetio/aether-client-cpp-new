///
///\file libsodium_ut_pwhash_scrypt_ll.h
///\brief Unit tests for the sodium library pwhash_scrypt_ll functions.
///
///\details
///
///\author Dmitriy Kiryanov dmitriyk@aethernet.io
///\version 1.0.0
///\date  15.07.2024
///
#ifndef THIRD_PARTY_LIBSODIUM_UT_PWHASH_SCRYPT_11_H
#define THIRD_PARTY_LIBSODIUM_UT_PWHASH_SCRYPT_11_H

#include "cmptest.h"

namespace pwhash_scrypt_ll
{    
    #define TEST_NAME51 "pwhash_scrypt_ll"

    static const char *   passwd1 = "";
    static const char *   salt1   = "";
    static const uint64_t N1      = 16U;
    static const uint32_t r1      = 1U;
    static const uint32_t p1      = 1U;

    static const char *   passwd2 = "password";
    static const char *   salt2   = "NaCl";
    static const uint64_t N2      = 1024U;
    static const uint32_t r2      = 8U;
    static const uint32_t p2      = 16U;

    static const char *   passwd3 = "pleaseletmein";
    static const char *   salt3   = "SodiumChloride";
    static const uint64_t N3      = 16384U;
    static const uint32_t r3      = 8U;
    static const uint32_t p3      = 1U;
    
    void _libsodium_ut_pwhash_scrypt_ll();
} // namespace generichash2

#endif /* THIRD_PARTY_LIBSODIUM_UT_PWHASH_SCRYPT_11_H */
