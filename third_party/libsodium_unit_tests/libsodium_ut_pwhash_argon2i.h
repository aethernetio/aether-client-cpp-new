///
///\file libsodium_ut_pwhash_argon2i.h
///\brief Unit tests for the sodium library pwhash_argon2i functions.
///
///\details
///
///\author Dmitriy Kiryanov dmitriyk@aethernet.io
///\version 1.0.0
///\date  15.07.2024
///
#ifndef THIRD_PARTY_LIBSODIUM_UT_PWHASH_ARGON2I_H
#define THIRD_PARTY_LIBSODIUM_UT_PWHASH_ARGON2I_H

#include "cmptest.h"

namespace pwhash_argon2i
{    
    #define TEST_NAME48 "pwhash_argon2i"

    #define OUT_LEN 128
    #define OPSLIMIT 3
    #define MEMLIMIT 5000000

    void _libsodium_ut_pwhash_argon2i();
} // namespace generichash2

#endif /* THIRD_PARTY_LIBSODIUM_UT_PWHASH_ARGON2I_H */
