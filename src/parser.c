/**
 * Copyright (c) 2016 Rishi Dhupar
 *
 * This software may be modified and distributed under the terms
 * of the MIT license.  See the LICENSE file for details.
 *
 * @brief
 *
 */

#include "parser.h"

#include <stdio.h>
#include <ctype.h>
#include <errno.h>
#include <math.h>
#include <limits.h>
#include <stdlib.h>
#include <arpa/inet.h>

bool parse_bool(const char *s)
{
    char ch = toupper((unsigned char)*s);
    switch (ch)
    {
        case 'Y': // Y, Yes
        case 'T': // T, True
        case '1':
            return true;
        default:
            return false;
    }
}

bool parse_integer(const char *s, long long *out)
{
    char *endptr;
    errno = 0;    // To distinguish success/failure after call

    *out = strtoll(s, &endptr, 0);

    if ((errno == ERANGE &&
         (*out == LONG_MAX || *out == LONG_MIN))
        || (errno != 0 && *out == 0))
    {
        perror("strtoll");
        return false;
    }

    if (endptr == s)
    {
        fprintf(stderr, "Parse error: no digits were found (%s)\n", s);
        return false;
    }

    return true;
}

bool parse_float(const char *s, double *out)
{
    char *endptr;
    errno = 0;    // To distinguish success/failure after call

    *out = strtod(s, &endptr);

    if ((errno == ERANGE &&
         (*out == HUGE_VAL || *out == -HUGE_VAL))
        || (errno != 0 && *out == 0.0))
    {
        perror("strtod");
        return false;
    }

    if (endptr == s)
    {
        fprintf(stderr, "Parse error: no digits were found (%s)\n", s);
        return false;
    }

    return true;
}

bool parse_ip_address(const char *s, uint32_t *out)
{
    int rc = inet_pton(AF_INET, s, out);

    return (rc == 1);
}