cmake_minimum_required(VERSION 3.16.0)

set(include_dirs "../libsodium/src/libsodium/include"
                 "../libsodium/src/libsodium/include/sodium"
                 "../libsodium/src/libsodium/include/sodium/private")

list(APPEND unit_tests_srcs
            "libsodium_unit_tests.cpp"
            "libsodium_ut_aead_aegis128l.cpp"
            "libsodium_ut_aead_aegis256.cpp"
            "libsodium_ut_aead_aes256gcm.cpp"
            "libsodium_ut_aead_aes256gcm2.cpp"
            "libsodium_ut_aead_chacha20poly1305.cpp"
            "libsodium_ut_aead_chacha20poly13052.cpp"
            "libsodium_ut_aead_xchacha20poly1305.cpp"
            "libsodium_ut_auth.cpp"
            "libsodium_ut_auth2.cpp"
            "libsodium_ut_auth3.cpp"
            "libsodium_ut_auth5.cpp"
            "libsodium_ut_auth6.cpp"
            "libsodium_ut_auth7.cpp"
            "libsodium_ut_box.cpp"
            "libsodium_ut_box_easy.cpp"
            "libsodium_ut_box_easy2.cpp"
            "libsodium_ut_box_seal.cpp"
            "libsodium_ut_box_seed.cpp"
            "libsodium_ut_box2.cpp"
            "libsodium_ut_box7.cpp"
            "libsodium_ut_box8.cpp"
            "libsodium_ut_chacha20.cpp"
            "libsodium_ut_codecs.cpp"
            "libsodium_ut_core_ed25519.cpp"
            "libsodium_ut_core_ed25519_h2c.cpp"
            "libsodium_ut_core_ristretto255.cpp"
            "libsodium_ut_core1.cpp"
            "libsodium_ut_core2.cpp"
            "libsodium_ut_core3.cpp"
            "libsodium_ut_core4.cpp"
            "libsodium_ut_core5.cpp"
            "libsodium_ut_core6.cpp"
            "libsodium_ut_ed25519_convert.cpp"
            "libsodium_ut_generichash.cpp"
            "libsodium_ut_generichash2.cpp"
            "libsodium_ut_generichash3.cpp"
            "libsodium_ut_hash.cpp"
            "libsodium_ut_hash3.cpp"
            "libsodium_ut_kdf.cpp"
            "libsodium_ut_kdf_hkdf.cpp"
            "libsodium_ut_keygen.cpp"
            "libsodium_ut_kx.cpp"
            "libsodium_ut_metamorphic.cpp"
            "libsodium_ut_misuse.cpp"
            "libsodium_ut_onetimeauth.cpp"
            "libsodium_ut_onetimeauth2.cpp"
            "libsodium_ut_onetimeauth7.cpp"
            "libsodium_ut_pwhash_argon2i.cpp"
            "libsodium_ut_pwhash_argon2id.cpp"
            "libsodium_ut_pwhash_scrypt.cpp"
            "libsodium_ut_pwhash_scrypt_ll.cpp"
            "libsodium_ut_randombytes.cpp"
            "libsodium_ut_scalarmult.cpp"
            "libsodium_ut_scalarmult_ed25519.cpp"
            "libsodium_ut_scalarmult_ristretto255.cpp"
            "libsodium_ut_scalarmult2.cpp"
            "libsodium_ut_scalarmult5.cpp"
            "libsodium_ut_scalarmult6.cpp"
            "libsodium_ut_scalarmult7.cpp"
            "libsodium_ut_scalarmult8.cpp"
            "libsodium_ut_secretbox.cpp"
            "libsodium_ut_secretbox_easy.cpp"
            "libsodium_ut_secretbox_easy2.cpp"
            "libsodium_ut_secretbox2.cpp"
            "libsodium_ut_secretbox7.cpp"
            "libsodium_ut_secretbox8.cpp"
            "libsodium_ut_secretstream_xchacha20poly1305.cpp"
            "libsodium_ut_shorthash.cpp"
            "libsodium_ut_sign.cpp"
            "libsodium_ut_sign2.cpp"
            "libsodium_ut_siphashx24.cpp"
            "libsodium_ut_sodium_utils.cpp"
            #"libsodium_ut_sodium_utils2.cpp"
            #"libsodium_ut_sodium_utils3.cpp"
            "libsodium_ut_sodium_version.cpp"
            "libsodium_ut_stream.cpp"
            "libsodium_ut_stream2.cpp"
            "libsodium_ut_stream3.cpp"
            "libsodium_ut_stream4.cpp"
            "libsodium_ut_verify1.cpp"
            "libsodium_ut_xchacha20.cpp"
            )

list(APPEND unit_tests_runner_srcs "unit_tests_runner.cpp")

if(NOT CM_PLATFORM)      
    project(libsodium_tests C CXX)
    
    add_executable(${PROJECT_NAME}
                   ${unit_tests_srcs}
                   ${unit_tests_runner_srcs})

    target_include_directories(${PROJECT_NAME} PRIVATE ${include_dirs})

    target_link_libraries(${PROJECT_NAME} PRIVATE sodium unity)

    add_test(NAME ${PROJECT_NAME} COMMAND $<TARGET_FILE:${PROJECT_NAME}>)
endif()
