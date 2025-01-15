///
///\file libsodium_ut_box_easy2.h
///\brief Unit tests for the sodium library box_easy2 functions.
///
///\details
///
///\author Dmitriy Kiryanov dmitriyk@aethernet.io
///\version 1.0.0
///\date  11.07.2024
///
#ifndef THIRD_PARTY_LIBSODIUM_UT_BOX_EASY2_H
#define THIRD_PARTY_LIBSODIUM_UT_BOX_EASY2_H

#include "cmptest.h"

namespace box_easy2
{
    #define TEST_NAME16 "box_easy2"
    
    static const unsigned char small_order_p[crypto_box_PUBLICKEYBYTES] = {
        0xe0, 0xeb, 0x7a, 0x7c, 0x3b, 0x41, 0xb8, 0xae, 0x16, 0x56, 0xe3,
        0xfa, 0xf1, 0x9f, 0xc4, 0x6a, 0xda, 0x09, 0x8d, 0xeb, 0x9c, 0x32,
        0xb1, 0xfd, 0x86, 0x62, 0x05, 0x16, 0x5f, 0x49, 0xb8, 0x00
    };

    void _libsodium_ut_box_easy2();
} // namespace box_easy2

#endif /* THIRD_PARTY_LIBSODIUM_UT_BOX_EASY2_H */
