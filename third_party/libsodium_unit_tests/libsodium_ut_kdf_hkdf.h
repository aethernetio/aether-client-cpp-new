///
///\file libsodium_ut_kdf_hkdf.h
///\brief Unit tests for the sodium library kdf_hkdf functions.
///
///\details
///
///\author Dmitriy Kiryanov dmitriyk@aethernet.io
///\version 1.0.0
///\date  15.07.2024
///
#ifndef THIRD_PARTY_LIBSODIUM_UT_KDF_HKDF_H
#define THIRD_PARTY_LIBSODIUM_UT_KDF_HKDF_H

#include "cmptest.h"

namespace kdf_hkdf
{    
    #define TEST_NAME40 "kdf_hkdf"
    
    void _libsodium_ut_kdf_hkdf();
} // namespace kdf_hkdf

#endif /* THIRD_PARTY_LIBSODIUM_UT_KDF_HKDF_H */
