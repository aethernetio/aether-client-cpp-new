///
///\file libsodium_ut_metamorphic.h
///\brief Unit tests for the sodium library metamorphic functions.
///
///\details
///
///\author Dmitriy Kiryanov dmitriyk@aethernet.io
///\version 1.0.0
///\date  15.07.2024
///
#ifndef THIRD_PARTY_LIBSODIUM_UT_METAMORPHIC_H
#define THIRD_PARTY_LIBSODIUM_UT_METAMORPHIC_H

#include "cmptest.h"

namespace metamorphic
{    
    #define TEST_NAME43 "metamorphic"

    #define MAXLEN 512
    #define MAX_ITER 1000

    void _libsodium_ut_metamorphic();
} // namespace metamorphic

#endif /* THIRD_PARTY_LIBSODIUM_UT_METAMORPHIC_H */
