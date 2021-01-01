/*
 * Test driver for generating keys.
 */
/*  Copyright The Mbed TLS Contributors
 *  SPDX-License-Identifier: Apache-2.0
 *
 *  Licensed under the Apache License, Version 2.0 (the "License"); you may
 *  not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 *  WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

#ifndef PSA_CRYPTO_TEST_DRIVERS_KEYGEN_H
#define PSA_CRYPTO_TEST_DRIVERS_KEYGEN_H

#if !defined(MBEDTLS_CONFIG_FILE)
#include "mbedtls/config.h"
#else
#include MBEDTLS_CONFIG_FILE
#endif

#if defined(PSA_CRYPTO_DRIVER_TEST)
#include <psa/crypto_driver_common.h>

typedef struct {
    /* If non-null, on success, copy this to the output. */
    void *forced_output;
    size_t forced_output_length;
    /* If not PSA_SUCCESS, return this error code instead of processing the
     * function call. */
    psa_status_t forced_status;
    /* Count the amount of times one of the keygen driver functions is called. */
    unsigned long hits;
} test_driver_keygen_hooks_t;

#define TEST_DRIVER_KEYGEN_INIT { NULL, 0, PSA_ERROR_NOT_SUPPORTED, 0 }
static inline test_driver_keygen_hooks_t test_driver_keygen_hooks_init( void )
{
    const test_driver_keygen_hooks_t v = TEST_DRIVER_KEYGEN_INIT;
    return( v );
}

extern test_driver_keygen_hooks_t test_driver_keygen_hooks;

psa_status_t test_transparent_generate_key(
    const psa_key_attributes_t *attributes,
    uint8_t *key, size_t key_size, size_t *key_length );

psa_status_t test_opaque_generate_key(
    const psa_key_attributes_t *attributes,
    uint8_t *key, size_t key_size, size_t *key_length );

#endif /* PSA_CRYPTO_DRIVER_TEST */
#endif /* PSA_CRYPTO_TEST_DRIVERS_KEYGEN_H */
