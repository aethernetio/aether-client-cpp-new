///
///\file libsodium_ut_pwhash_argon2id.h
///\brief Unit tests for the sodium library pwhash_argon2id functions.
///
///\details
///
///\author Dmitriy Kiryanov dmitriyk@aethernet.io
///\version 1.0.0
///\date  15.07.2024
///
#ifndef THIRD_PARTY_LIBSODIUM_UT_PWHASH_ARGON2ID_H
#define THIRD_PARTY_LIBSODIUM_UT_PWHASH_ARGON2ID_H

#include "cmptest.h"

namespace pwhash_argon2id
{    
    #define TEST_NAME49 "pwhash_argon2id"

    #define OUT_LEN 128
    #define OPSLIMIT 3
    #define MEMLIMIT 5000000

    void _libsodium_ut_pwhash_argon2id();
} // namespace pwhash_argon2id

#endif /* THIRD_PARTY_LIBSODIUM_UT_PWHASH_ARGON2ID_H */
