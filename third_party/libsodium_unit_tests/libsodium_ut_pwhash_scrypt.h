///
///\file libsodium_ut_pwhash_scrypt.h
///\brief Unit tests for the sodium library pwhash_scrypt functions.
///
///\details
///
///\author Dmitriy Kiryanov dmitriyk@aethernet.io
///\version 1.0.0
///\date  15.07.2024
///
#ifndef THIRD_PARTY_LIBSODIUM_UT_PWHASH_SCRYPT_H
#define THIRD_PARTY_LIBSODIUM_UT_PWHASH_SCRYPT_H

#include "cmptest.h"

namespace pwhash_scrypt
{    
    #define TEST_NAME50 "pwhash_scrypt"

    #define OUT_LEN 128
    #define OPSLIMIT 1000000
    #define MEMLIMIT 10000000

    void _libsodium_ut_pwhash_scrypt();
} // namespace pwhash_scrypt

#endif /* THIRD_PARTY_LIBSODIUM_UT_PWHASH_SCRYPT_H */
