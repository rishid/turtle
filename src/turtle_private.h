/**
 * Copyright (c) 2016 Rishi Dhupar
 *
 * This software may be modified and distributed under the terms
 * of the MIT license.  See the LICENSE file for details.
 *
 * @brief private structures and macros for turtle library
 *
 */
#ifndef TURTLE_PRIVATE_H_
#define TURTLE_PRIVATE_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "ffi.h"

#include <stdbool.h>
#include <stdint.h>

/// List of possible argument types supported
enum arg_type_t
{
    ARG_BOOL,    // %b (bool)
    ARG_CHAR,    // %c (char)
    ARG_FLOAT,   // %f (double)
    ARG_INTEGER, // %d (int64_t)
    ARG_IP_ADDR, // %I (uint32_t)
    ARG_STRING,  // %s (char *)
};

/// Union to store each argument type supported
union arg_value
{
    bool         boolarg;
    char         chararg;
    double       doublearg;
    long long    longlongarg;
    uint32_t     ipaddrarg;
    const char * pchararg;
};

/// structure which holds the meta data about each argument
struct arg_t
{
    enum arg_type_t type;
    char switch_arg;
    bool required;
    union arg_value default_value;
    bool validation;
    long int min_valid;
    long int max_valid;
};

#ifdef __cplusplus
}
#endif

#endif /* TOKENIZER_H_ */
