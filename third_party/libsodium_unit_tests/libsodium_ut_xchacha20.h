///
///\file libsodium_ut_xchacha20.h
///\brief Unit tests for the sodium library xchacha20 functions.
///
///\details
///
///\author Dmitriy Kiryanov dmitriyk@aethernet.io
///\version 1.0.0
///\date  16.07.2024
///
#ifndef THIRD_PARTY_LIBSODIUM_UT_XCHACHA20_H
#define THIRD_PARTY_LIBSODIUM_UT_XCHACHA20_H

#include "cmptest.h"

namespace xchacha20
{    
    #define TEST_NAME81 "xchacha20"

    typedef struct HChaCha20TV_ {
        const char key[crypto_core_hchacha20_KEYBYTES * 2 + 1];
        const char in[crypto_core_hchacha20_INPUTBYTES * 2 + 1];
        const char out[crypto_core_hchacha20_OUTPUTBYTES * 2 + 1];
    } HChaCha20TV;

    static const unsigned char small_order_p[crypto_scalarmult_BYTES] = {
        0xe0, 0xeb, 0x7a, 0x7c, 0x3b, 0x41, 0xb8, 0xae, 0x16, 0x56, 0xe3,
        0xfa, 0xf1, 0x9f, 0xc4, 0x6a, 0xda, 0x09, 0x8d, 0xeb, 0x9c, 0x32,
        0xb1, 0xfd, 0x86, 0x62, 0x05, 0x16, 0x5f, 0x49, 0xb8, 0x00
    };

    void _libsodium_ut_xchacha20();
} // namespace xchacha20

#endif /* THIRD_PARTY_LIBSODIUM_UT_XCHACHA20_H */
