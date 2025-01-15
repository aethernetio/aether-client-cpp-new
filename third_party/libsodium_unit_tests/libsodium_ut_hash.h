///
///\file libsodium_ut_hash.h
///\brief Unit tests for the sodium library hash functions.
///
///\details
///
///\author Dmitriy Kiryanov dmitriyk@aethernet.io
///\version 1.0.0
///\date  15.07.2024
///
#ifndef THIRD_PARTY_LIBSODIUM_UT_HASH_H
#define THIRD_PARTY_LIBSODIUM_UT_HASH_H

#include "cmptest.h"

namespace hash
{    
    #define TEST_NAME37 "hash"

    static unsigned char x[] = "testing\n";
    static unsigned char x2[] =
        "The Conscience of a Hacker is a small essay written January 8, 1986 by a "
        "computer security hacker who went by the handle of The Mentor, who "
        "belonged to the 2nd generation of Legion of Doom.";
    static unsigned char h[crypto_hash_BYTES];

    void _libsodium_ut_hash();
} // namespace hash

#endif /* THIRD_PARTY_LIBSODIUM_UT_HASH_H */
