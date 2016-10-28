/**
 * Copyright (c) 2016 Rishi Dhupar
 *
 * This software may be modified and distributed under the terms
 * of the MIT license.  See the LICENSE file for details.
 *
 * @brief Simple micro benchmark for turtle library
 *
 */

#include "turtle.h"

#include <time.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

static void _no_params(void)
{
    asm("nop");
}

static void _bcdfIs(bool b, char c, int64_t d, double f, uint32_t I, const char *s)
{
    asm("nop");
}

static void _int10_string10(int a0, int a1, int a2, int a3, int a4,
                            const char *s0, const char *s1, const char *s2,
                            const char *s3, const char *s4,
                            int a5, int a6, int a7, int a8, int a9,
                            const char *s5, const char *s6, const char *s7,
                            const char *s8, const char *s9)
{
    asm("nop");
}

static turtle_cmd_list_t _cmds = {
    TURTLE_CMD_NP("_no_params", NULL, _no_params),
    TURTLE_CMD("bcdfIs", "%b %c %d %f %I %s", NULL, _bcdfIs),
    TURTLE_CMD("int10string10", "%d %d %d %d %d %s %s %s %s %s %d %d %d %d %d %s %s %s %s %s", NULL, _int10_string10),
    { NULL, },
};

#define NUM_ITERATIONS (20000000)

static void _run_benchmark(const char *execute_str)
{
    struct timespec start, end;

    clock_gettime(CLOCK_MONOTONIC_RAW, &start);

    for (int i = 0; i < NUM_ITERATIONS; i++)
    {
        turtle_execute(execute_str);
    }

    clock_gettime(CLOCK_MONOTONIC_RAW, &end);

    uint64_t duration;
    duration = (1000000000UL * (end.tv_sec - start.tv_sec)) + (end.tv_nsec - start.tv_nsec);

    printf("Command: %s\n", execute_str);
    printf("- Iterations              : %lu\n", NUM_ITERATIONS);
    printf("- Time per iteration (ns) : %lu\n", duration / NUM_ITERATIONS);
    printf("- Items/second            : %.03fM\n", (double)NUM_ITERATIONS / duration * 1000L);
    printf("\n");
}

int main()
{
    turtle_init(_cmds);

    _run_benchmark("_no_params");
    _run_benchmark("bcdfIs f a 54321 3.14 1.2.3.4 'my cool string'");
    _run_benchmark("int10string10 1 2 3 4 5 abc def ghi jkl mno 6 7 8 9 0 abc def ghi jkl mno");

    return 0;
}
