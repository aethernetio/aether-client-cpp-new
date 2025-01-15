///
///\file libsodium_ut_box_seed.h
///\brief Unit tests for the sodium library box_seed functions.
///
///\details
///
///\author Dmitriy Kiryanov dmitriyk@aethernet.io
///\version 1.0.0
///\date  11.07.2024
///
#ifndef THIRD_PARTY_LIBSODIUM_UT_BOX_SEED_H
#define THIRD_PARTY_LIBSODIUM_UT_BOX_SEED_H

#include "cmptest.h"

namespace box_seed
{
    #define TEST_NAME18 "box_seed"
    
    static unsigned char seed[32] = { 0x77, 0x07, 0x6d, 0x0a, 0x73, 0x18, 0xa5,
                                      0x7d, 0x3c, 0x16, 0xc1, 0x72, 0x51, 0xb2,
                                      0x66, 0x45, 0xdf, 0x4c, 0x2f, 0x87, 0xeb,
                                      0xc0, 0x99, 0x2a, 0xb1, 0x77, 0xfb, 0xa5,
                                      0x1d, 0xb9, 0x2c, 0x2a };
                                      
    void _libsodium_ut_box_seed();
} // namespace box_seed

#endif /* THIRD_PARTY_LIBSODIUM_UT_BOX_SEED_H */
