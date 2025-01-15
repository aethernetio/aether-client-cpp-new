///
///\file libsodium_ut_siphashx24.h
///\brief Unit tests for the sodium library siphashx24 functions.
///
///\details
///
///\author Dmitriy Kiryanov dmitriyk@aethernet.io
///\version 1.0.0
///\date  15.07.2024
///
#ifndef THIRD_PARTY_LIBSODIUM_UT_SIPHASHX24_H
#define THIRD_PARTY_LIBSODIUM_UT_SIPHASHX24_H

#include "cmptest.h"

namespace siphashx24
{    
    #define TEST_NAME71 "siphashx24"

    #define MAXLEN 64

    void _libsodium_ut_siphashx24();
} // namespace siphashx24

#endif /* THIRD_PARTY_LIBSODIUM_UT_SIPHASHX24_H */
