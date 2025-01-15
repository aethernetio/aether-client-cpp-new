///
///\file libsodium_ut_box7.h
///\brief Unit tests for the sodium library box7 functions.
///
///\details
///
///\author Dmitriy Kiryanov dmitriyk@aethernet.io
///\version 1.0.0
///\date  15.07.2024
///
#ifndef THIRD_PARTY_LIBSODIUM_UT_BOX7_H
#define THIRD_PARTY_LIBSODIUM_UT_BOX7_H

#include "cmptest.h"

namespace box7
{
    #define TEST_NAME20 "box7"

    static unsigned char alicesk[crypto_box_SECRETKEYBYTES];
    static unsigned char alicepk[crypto_box_PUBLICKEYBYTES];
    static unsigned char bobsk[crypto_box_SECRETKEYBYTES];
    static unsigned char bobpk[crypto_box_PUBLICKEYBYTES];
    static unsigned char n[crypto_box_NONCEBYTES];
    
    void _libsodium_ut_box7();
} // namespace box7

#endif /* THIRD_PARTY_LIBSODIUM_UT_BOX7_H */
