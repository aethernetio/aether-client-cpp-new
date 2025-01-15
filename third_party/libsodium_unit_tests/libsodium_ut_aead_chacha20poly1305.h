///
///\file libsodium_ut_aead_chacha20poly1305.h
///\brief Unit tests for the sodium library aead_aegis128l functions.
///
///\details
///
///\author Dmitriy Kiryanov dmitriyk@aethernet.io
///\version 1.0.0
///\date  16.04.2024
///
#ifndef THIRD_PARTY_LIBSODIUM_UT_AEAD_CHACHA20POLY1305_H
#define THIRD_PARTY_LIBSODIUM_UT_AEAD_CHACHA20POLY1305_H

#include "cmptest.h"

namespace aead_chacha20poly1305
{
    #define TEST_NAME5 "aead_chacha20poly1305"

    void _libsodium_ut_aead_chacha20poly1305();
} // namespace aead_chacha20poly1305

#endif /* THIRD_PARTY_LIBSODIUM_UT_AEAD_CHACHA20POLY1305_H */
