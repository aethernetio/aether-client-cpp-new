///
///\file libsodium_ut_kx.h
///\brief Unit tests for the sodium library kx functions.
///
///\details
///
///\author Dmitriy Kiryanov dmitriyk@aethernet.io
///\version 1.0.0
///\date  15.07.2024
///
#ifndef THIRD_PARTY_LIBSODIUM_UT_KX_H
#define THIRD_PARTY_LIBSODIUM_UT_KX_H

#include "cmptest.h"

namespace kx
{    
    #define TEST_NAME42 "kx"

    static const unsigned char small_order_p[crypto_scalarmult_BYTES] = {
        0xe0, 0xeb, 0x7a, 0x7c, 0x3b, 0x41, 0xb8, 0xae, 0x16, 0x56, 0xe3,
        0xfa, 0xf1, 0x9f, 0xc4, 0x6a, 0xda, 0x09, 0x8d, 0xeb, 0x9c, 0x32,
        0xb1, 0xfd, 0x86, 0x62, 0x05, 0x16, 0x5f, 0x49, 0xb8, 0x00
    };

    void _libsodium_ut_kx();
} // namespace kx

#endif /* THIRD_PARTY_LIBSODIUM_UT_KX_H */
