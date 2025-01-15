///
///\file libsodium_ut_shorthash.h
///\brief Unit tests for the sodium library shorthash functions.
///
///\details
///
///\author Dmitriy Kiryanov dmitriyk@aethernet.io
///\version 1.0.0
///\date  15.07.2024
///
#ifndef THIRD_PARTY_LIBSODIUM_UT_SHORTHASH_H
#define THIRD_PARTY_LIBSODIUM_UT_SHORTHASH_H

#include "cmptest.h"

namespace shorthash
{    
    #define TEST_NAME68 "shorthash"

    #define MAXLEN 64

    void _libsodium_ut_shorthash();
} // namespace scalarmult5

#endif /* THIRD_PARTY_LIBSODIUM_UT_SHORTHASH_H */
