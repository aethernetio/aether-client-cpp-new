///
///\file libsodium_ut_scalarmult_ristretto255.h
///\brief Unit tests for the sodium library scalarmult_ristretto255 functions.
///
///\details
///
///\author Dmitriy Kiryanov dmitriyk@aethernet.io
///\version 1.0.0
///\date  15.07.2024
///
#ifndef THIRD_PARTY_LIBSODIUM_UT_SCALARMULT_RISTRETTO255_H
#define THIRD_PARTY_LIBSODIUM_UT_SCALARMULT_RISTRETTO255_H

#include "cmptest.h"

namespace scalarmult_ristretto255
{
    #define TEST_NAME55 "scalarmult_ristretto255"

    #define B_HEX "e2f2ae0a6abc4e71a884a961c500515f58e30b6aa582dd8db6a65945e08d2d76"

    void _libsodium_ut_scalarmult_ristretto255();
} // namespace scalarmult_ristretto255

#endif /* THIRD_PARTY_LIBSODIUM_UT_SCALARMULT_RISTRETTO255_H */
