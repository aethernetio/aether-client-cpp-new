///
///\file libsodium_ut_sodium_utils3.h
///\brief Unit tests for the sodium library sodium_utils3 functions.
///
///\details
///
///\author Dmitriy Kiryanov dmitriyk@aethernet.io
///\version 1.0.0
///\date  16.07.2024
///
#ifndef THIRD_PARTY_LIBSODIUM_UT_SODIUM_UTILS3_H
#define THIRD_PARTY_LIBSODIUM_UT_SODIUM_UTILS3_H

#include "cmptest.h"

namespace sodium_utils3
{    
    #include <stdlib.h>
    #include <sys/types.h>

    #include <limits.h>
    #ifdef HAVE_CATCHABLE_SEGV
    # include <signal.h>
    #endif
    #ifndef _WIN32
    # include <unistd.h>
    #endif

    #define TEST_NAME74 "sodium_utils3"

    #ifdef __SANITIZE_ADDRESS__
    # warning The sodium_utils3 test is expected to fail with address sanitizer
    #endif

    __attribute__((noreturn)) static void
    segv_handler(int sig)
    {
        (void) sig;

        printf("Intentional segfault / bus error caught\n");
        printf("OK\n");
    #ifdef SIG_DFL
    # ifdef SIGPROT
        signal(SIGPROT, SIG_DFL);
    # endif
    # ifdef SIGSEGV
        signal(SIGSEGV, SIG_DFL);
    # endif
    # ifdef SIGBUS
        signal(SIGBUS, SIG_DFL);
    # endif
    # ifdef SIGABRT
        signal(SIGABRT, SIG_DFL);
    # endif
    #endif
        _exit(0);
    }

    void _libsodium_ut_sodium_utils3();
} // namespace sodium_utils3

#endif /* THIRD_PARTY_LIBSODIUM_UT_SODIUM_UTILS3_H */
