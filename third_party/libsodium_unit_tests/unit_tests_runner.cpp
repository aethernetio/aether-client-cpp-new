///
///\file functions_unit_tests.cpp
///\brief AetherNet library example
/// Unit tests for the CI/CD example
///
/// This example code is in the Public Domain (or CC0 licensed, at your option.)
///
/// Unless required by applicable law or agreed to in writing, this
/// software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
/// CONDITIONS OF ANY KIND, either express or implied.
///
///\author Dmitriy Kiryanov dmitriyk@aethernet.io
///\version 1.0.0
///\date  11.07.2024
///
#include "unity.h"
#include "unit_tests_runner.h"
#include "libsodium_unit_tests.h"


///
///\brief setUp.
///\param[in] void
///\return void
///
void setUp(void)
{
    UNITY_OUTPUT_CHAR('\n');
    UnityPrint("Unity setUp tests");
    UNITY_OUTPUT_CHAR('\n');
}

///
///\brief tearDown.
///\param[in] void
///\return void
///
void tearDown(void)
{
    UNITY_OUTPUT_CHAR('\n');
    UnityPrint("Unity tearDown tests");
    UNITY_OUTPUT_CHAR('\n');
}


///
///\brief main.
///\param[in] void
///\return int
///
int main(void)
{
    return libsodium_unit_tests_runner();
}