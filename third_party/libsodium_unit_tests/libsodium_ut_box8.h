///
///\file libsodium_ut_box8.h
///\brief Unit tests for the sodium library box8 functions.
///
///\details
///
///\author Dmitriy Kiryanov dmitriyk@aethernet.io
///\version 1.0.0
///\date  15.07.2024
///
#ifndef THIRD_PARTY_LIBSODIUM_UT_BOX8_H
#define THIRD_PARTY_LIBSODIUM_UT_BOX8_H

#include "cmptest.h"

namespace box8
{
    #define TEST_NAME21 "box8"

    static unsigned char alicesk[crypto_box_SECRETKEYBYTES];
    static unsigned char alicepk[crypto_box_PUBLICKEYBYTES];
    static unsigned char bobsk[crypto_box_SECRETKEYBYTES];
    static unsigned char bobpk[crypto_box_PUBLICKEYBYTES];
    static unsigned char n[crypto_box_NONCEBYTES];
    
    void _libsodium_ut_box8();
} // namespace box8

#endif /* THIRD_PARTY_LIBSODIUM_UT_BOX8_H */
