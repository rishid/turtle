/**
 * Copyright (c) 2016 Rishi Dhupar
 *
 * This software may be modified and distributed under the terms
 * of the MIT license.  See the LICENSE file for details.
 *
 * @brief private structures and macros for turtle library
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

static long int p3_call = 0;

static void _no_params(void)
{
  _flag = true;
}

static void print1(int x)
{
    printf("enter %s\n", __FUNCTION__);
    printf("x = %d\n", x);
}

static void print2(int x, int y)
{
    printf("enter %s\n", __FUNCTION__);
    printf("x = %d, y = %d\n", x, y);
}

static void print3(const char *a, int b, bool c)
{
    //printf("enter %s\n", __FUNCTION__);
    //printf("a = %s b = %d c = %d\n", a, b, c);
    p3_call++;
}

static void print5(int a0, int a1, int a2,
                   int a3,int a4,int a5,int a6,int a7,int a8,int a9,int a10,
                   int a11,int a12,int a13,int a14,int a15,int a16,int a17,
                   int a18,int a19)
{
    p3_call++;
}

static void _try_bool1(bool b)
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
    TURTLE_CMD("try_bool", "%b", NULL, _try_bool1),
    TURTLE_CMD("bcdfIs", "%b %c %d %f %I %s", NULL, _bcdfIs),
#if 0
    { "print1", "%d", "help1", print1 },
    { "print2", "%d %d", "help2", print2 },
    { "print1", "%d", "help1", print1 },
    { "print3", "%s %d %b", "help3", print3 },
    { "print4", "", "help4", print0 },
    { "print5", "%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d", "help5", print5 },
#endif
    //{ "print0", NULL, "help0", 0, false, print0 },
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
    //turtle_cmd_list_t cmds = {    
    //  TURTLE_CMD_NP("_no_params", NULL, _no_params),
    //  { NULL, },
    //};
    //ASSERT_TRUE(turtle_init(cmds));
    _flag = false;
    ASSERT_TRUE(turtle_execute("_no_params"));
    ASSERT_TRUE(_flag);
}

CTEST2(suite, bool1) {    
    ASSERT_TRUE(turtle_execute("try_bool t"));
    ASSERT_TRUE(_values[0].boolarg);
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

int main(int argc, const char *argv[])
{
    return ctest_main(argc, argv);
}

#if 0
int main()
{
    turtle_init(cmds);

    printf("\n\n");
    printf("-- it's turtle time!\n");

    turtle_execute("print0");

    turtle_execute("try_bool t");
    turtle_execute("try_bool f");
    turtle_execute("try_bool 1");
    turtle_execute("try_bool w");
    
    turtle_execute("help");

#if 0
    turtle_execute("print1 4");

    turtle_execute("print2 7 8");

    struct timespec start, end;
#define NUM_ITERATIONS (20000000)

    clock_gettime(CLOCK_MONOTONIC_RAW, &start);
    for (int i = 0; i < NUM_ITERATIONS; i++)
    {
        //turtle_execute("print4");
        turtle_execute("print5 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20");
    }
    clock_gettime(CLOCK_MONOTONIC_RAW, &end);

    uint64_t duration = (1000000000UL * (end.tv_sec - start.tv_sec)) + (end.tv_nsec - start.tv_nsec);

    printf("%lu p3_call\n", p3_call);
    printf("%lu iterations took %lu nanoseconds\n", NUM_ITERATIONS, duration);
    printf("%.03f million items/sec\n", (double)NUM_ITERATIONS / duration * 1000L);
#endif
    return 0;
}
#endif
