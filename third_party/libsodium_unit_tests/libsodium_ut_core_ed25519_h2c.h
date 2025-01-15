///
///\file libsodium_ut_core_ed25519_h2c.h
///\brief Unit tests for the sodium library core_ed25519_h2c functions.
///
///\details
///
///\author Dmitriy Kiryanov dmitriyk@aethernet.io
///\version 1.0.0
///\date  15.07.2024
///
#ifndef THIRD_PARTY_LIBSODIUM_UT_CORE_ED25519_H2C_H
#define THIRD_PARTY_LIBSODIUM_UT_CORE_ED25519_H2C_H

#include "cmptest.h"

namespace core_ed25519_h2c
{
    #define TEST_NAME25 "core_ed25519_h2c"

    typedef struct TestData_ {
        int         ro;
        const char *msg;
        const char  tv_y[64 + 1];
    } TestData;

    static TestData test_data[] = {
        { 0, "",
          "222e314d04a4d5725e9f2aff9fb2a6b69ef375a1214eb19021ceab2d687f0f9b" },
        { 0, "abc",
          "67732d50f9a26f73111dd1ed5dba225614e538599db58ba30aaea1f5c827fa42" },
        { 0, "abcdef0123456789",
          "2f8a6c24dd1adde73909cada6a4a137577b0f179d336685c4a955a0a8e1a86fb" },
        { 0,
          "q128_"
          "qqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqq"
          "qqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqq",
          "2af6ff6ef5ebba128b0774f4296cb4c2279a074658b083b8dcca91f57a603450" },
        { 0,
          "a512_"
          "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
          "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
          "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
          "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
          "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
          "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
          "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
          "aaaaaaaa",
          "2c90c3d39eb18ff291d33441b35f3262cdd307162cc97c31bfcc7a4245891a37" },
        { 1, "",
          "09a6c8561a0b22bef63124c588ce4c62ea83a3c899763af26d795302e115dc21" },
        { 1, "abc",
          "1a8395b88338f22e435bbd301183e7f20a5f9de643f11882fb237f88268a5531" },
        { 1, "abcdef0123456789",
          "53060a3d140e7fbcda641ed3cf42c88a75411e648a1add71217f70ea8ec561a6" },
        { 1,
          "q128_"
          "qqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqq"
          "qqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqq",
          "2eca15e355fcfa39d2982f67ddb0eea138e2994f5956ed37b7f72eea5e89d2f7" },
        { 1,
          "a512_"
          "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
          "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
          "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
          "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
          "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
          "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
          "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
          "aaaaaaaa",
          "6dc2fc04f266c5c27f236a80b14f92ccd051ef1ff027f26a07f8c0f327d8f995" }
    };

    #define H2CHASH crypto_core_ed25519_H2CSHA512
    
    void _libsodium_ut_core_ed25519_h2c();
} // namespace core_ed25519_h2c

#endif /* THIRD_PARTY_LIBSODIUM_UT_CORE_ED25519_H2C_H */
