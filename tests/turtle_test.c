/**
 * Copyright (c) 2016 Rishi Dhupar
 *
 * This software may be modified and distributed under the terms
 * of the MIT license.  See the LICENSE file for details.
 *
 * @brief API level tests for turtle library
 *
 */

#include "turtle.h"
#include "turtle_private.h"

#include <time.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#define CTEST_MAIN
#define CTEST_SEGFAULT
#include "ctest.h"

static union arg_value _values[TURTLE_MAX_CMD_PARAMS];
static bool _flag;

static void _no_params(void)
{
    _flag = true;
}

static void print5(int a0, int a1, int a2,
                   int a3,int a4,int a5,int a6,int a7,int a8,int a9,int a10,
                   int a11,int a12,int a13,int a14,int a15,int a16,int a17,
                   int a18,int a19)
{
    asm("nop");
}

static void _bool1(bool b)
{
    _values[0].boolarg = b;
}

static void _bcdfIs(bool b, char c, int64_t d, double f, uint32_t I, const char *s)
{
    _values[0].boolarg      = b;
    _values[1].chararg      = c;
    _values[2].longlongarg  = d;
    _values[3].doublearg    = f;
    _values[4].ipaddrarg    = I;
    _values[5].pchararg     = s;
}

static turtle_cmd_list_t _cmds = {
    TURTLE_CMD_NP("_no_params", NULL, _no_params),
    TURTLE_CMD("bool1", "%b", NULL, _bool1),
    TURTLE_CMD("bcdfIs", "%b %c %d %f %I %s", NULL, _bcdfIs),
    { NULL, },
};

CTEST_DATA(suite) {
};

CTEST_SETUP(suite) {
    ASSERT_TRUE(turtle_init(_cmds));
}

CTEST_TEARDOWN(suite) {
    ASSERT_TRUE(turtle_deinit());
}

CTEST2(suite, no_params) {
    _flag = false;
    ASSERT_TRUE(turtle_execute("_no_params"));
    ASSERT_TRUE(_flag);
}

CTEST2(suite, bool1) {
    ASSERT_TRUE(turtle_execute("bool1 t"));
    ASSERT_TRUE(_values[0].boolarg);
}

CTEST2(suite, bad_cmd) {
    ASSERT_FALSE(turtle_execute("foobar"));
    ASSERT_FALSE(turtle_execute("foobar t"));
}

CTEST2(suite, all_types) {
    ASSERT_TRUE(turtle_execute("bcdfIs f a 54321 3.14 1.2.3.4 'my cool string'"));
    ASSERT_FALSE(_values[0].boolarg);
    ASSERT_EQUAL('a', _values[1].chararg);
    ASSERT_EQUAL(54321, _values[2].longlongarg);
    ASSERT_EQUAL(3.14, _values[3].doublearg);
    ASSERT_EQUAL(0x04030201, _values[4].ipaddrarg);
    ASSERT_STR("my cool string", _values[5].pchararg);
}

CTEST(suite, bad_param_str_0) {
    turtle_cmd_list_t cmds = {
        TURTLE_CMD("foobar", "%", NULL, _no_params),
        { NULL, },
    };
    ASSERT_FALSE(turtle_init(cmds));
    ASSERT_TRUE(turtle_deinit());
}

CTEST(suite, invalid_param_specifier) {
    turtle_cmd_list_t cmds = {
        TURTLE_CMD("foobar", "%z", NULL, _no_params),
        { NULL, },
    };
    ASSERT_FALSE(turtle_init(cmds));
    ASSERT_TRUE(turtle_deinit());
}

int main(int argc, const char *argv[])
{
    return ctest_main(argc, argv);
}
