/**
 * Copyright (c) 2016 Rishi Dhupar
 *
 * This software may be modified and distributed under the terms
 * of the MIT license.  See the LICENSE file for details.
 *
 * @brief
 *
 */

#ifndef PARSER_H_
#define PARSER_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>
 
bool parse_bool(const char *s);

bool parse_integer(const char *s, long long *out);

bool parse_float(const char *s, double *out);

bool parse_ip_address(const char *s, uint32_t *out);

#ifdef __cplusplus
}
#endif

#endif /* PARSER_H_ */