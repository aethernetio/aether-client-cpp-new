///
///\file libsodium_ut_ed25519_convert.h
///\brief Unit tests for the sodium library ed25519_convert functions.
///
///\details
///
///\author Dmitriy Kiryanov dmitriyk@aethernet.io
///\version 1.0.0
///\date  15.07.2024
///
#ifndef THIRD_PARTY_LIBSODIUM_UT_ED25519_CONVERT_H
#define THIRD_PARTY_LIBSODIUM_UT_ED25519_CONVERT_H

#include "cmptest.h"

namespace ed25519_convert
{    
    #define TEST_NAME33 "ed25519_convert"

    static const unsigned char keypair_seed[crypto_sign_ed25519_SEEDBYTES] = {
        0x42, 0x11, 0x51, 0xa4, 0x59, 0xfa, 0xea, 0xde, 0x3d, 0x24, 0x71,
        0x15, 0xf9, 0x4a, 0xed, 0xae, 0x42, 0x31, 0x81, 0x24, 0x09, 0x5a,
        0xfa, 0xbe, 0x4d, 0x14, 0x51, 0xa5, 0x59, 0xfa, 0xed, 0xee
    };

    void _libsodium_ut_ed25519_convert();
} // namespace ed25519_convert

#endif /* THIRD_PARTY_LIBSODIUM_UT_ED25519_CONVERT_H */
