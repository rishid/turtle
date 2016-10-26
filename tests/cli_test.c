
#include "cli_dispatcher/cli_dispatcher.h"

#include <time.h>
#include <stdio.h>


long int p3_call = 0;

static void print0(void)
{
    //printf("enter %s\n", __FUNCTION__);
    p3_call++;
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


// cli_cmd_t cmds[] =
cli_cmd_list_t cmds = {
    { "print0", NULL, "help0", 0, false, print0 },
    { "print1", "%d", "help1", 0, false, print1 },
    { "print2", "%d %d", "help2", 0, false, print2 },
    { "print1", "%d", "help1", 0, false, print1 },
    { "print2", "%d %d", "help2", 0, false, print2 },
    { "print1", "%d", "help1", 0, false, print1 },
    { "print2", "%d %d", "help2", 0, false, print2 },
    { "print1", "%d", "help1", 0, false, print1 },
    { "print2", "%d %d", "help2", 0, false, print2 },
    { "print1", "%d", "help1", 0, false, print1 },
    { "print2", "%d %d", "help2", 0, false, print2 },
    { "print1", "%d", "help1", 0, false, print1 },
    { "print2", "%d %d", "help2", 0, false, print2 },
    { "print1", "%d", "help1", 0, false, print1 },
    { "print2", "%d %d", "help2", 0, false, print2 },
    { "print1", "%d", "help1", 0, false, print1 },
    { "print2", "%d %d", "help2", 0, false, print2 },
    { "print1", "%d", "help1", 0, false, print1 },
    { "print2", "%d %d", "help2", 0, false, print2 },
    { "print1", "%d", "help1", 0, false, print1 },
    { "print2", "%d %d", "help2", 0, false, print2 },
    { "print1", "%d", "help1", 0, false, print1 },
    { "print2", "%d %d", "help2", 0, false, print2 },
    { "print1", "%d", "help1", 0, false, print1 },
    { "print2", "%d %d", "help2", 0, false, print2 },
    { "print1", "%d", "help1", 0, false, print1 },
    { "print2", "%d %d", "help2", 0, false, print2 },
    { "print1", "%d", "help1", 0, false, print1 },
    { "print2", "%d %d", "help2", 0, false, print2 },
    { "print1", "%d", "help1", 0, false, print1 },
    { "print2", "%d %d", "help2", 0, false, print2 },
    { "print1", "%d", "help1", 0, false, print1 },
    { "print2", "%d %d", "help2", 0, false, print2 },
    { "print1", "%d", "help1", 0, false, print1 },
    { "print2", "%d %d", "help2", 0, false, print2 },
    { "print1", "%d", "help1", 0, false, print1 },
    { "print2", "%d %d", "help2", 0, false, print2 },
    { "print1", "%d", "help1", 0, false, print1 },
    { "print2", "%d %d", "help2", 0, false, print2 },
    { "print1", "%d", "help1", 0, false, print1 },
    { "print2", "%d %d", "help2", 0, false, print2 },
    { "print1", "%d", "help1", 0, false, print1 },
    { "print2", "%d %d", "help2", 0, false, print2 },
    { "print1", "%d", "help1", 0, false, print1 },
    { "print2", "%d %d", "help2", 0, false, print2 },
    { "print1", "%d", "help1", 0, false, print1 },
    { "print2", "%d %d", "help2", 0, false, print2 },
    { "print1", "%d", "help1", 0, false, print1 },
    { "print2", "%d %d", "help2", 0, false, print2 },
    { "print1", "%d", "help1", 0, false, print1 },
    { "print2", "%d %d", "help2", 0, false, print2 },
    { "print1", "%d", "help1", 0, false, print1 },
    { "print2", "%d %d", "help2", 0, false, print2 },
    { "print1", "%d", "help1", 0, false, print1 },
    { "print2", "%d %d", "help2", 0, false, print2 },
    { "print1", "%d", "help1", 0, false, print1 },
    { "print2", "%d %d", "help2", 0, false, print2 },
    { "print1", "%d", "help1", 0, false, print1 },
    { "print2", "%d %d", "help2", 0, false, print2 },
    { "print1", "%d", "help1", 0, false, print1 },
    { "print2", "%d %d", "help2", 0, false, print2 },
    { "print1", "%d", "help1", 0, false, print1 },
    { "print2", "%d %d", "help2", 0, false, print2 },
    { "print1", "%d", "help1", 0, false, print1 },
    { "print2", "%d %d", "help2", 0, false, print2 },
    { "print1", "%d", "help1", 0, false, print1 },
    { "print2", "%d %d", "help2", 0, false, print2 },
    { "print1", "%d", "help1", 0, false, print1 },
    { "print2", "%d %d", "help2", 0, false, print2 },
    { "print1", "%d", "help1", 0, false, print1 },
    { "print2", "%d %d", "help2", 0, false, print2 },
    { "print1", "%d", "help1", 0, false, print1 },
    { "print2", "%d %d", "help2", 0, false, print2 },
    { "print1", "%d", "help1", 0, false, print1 },
    { "print2", "%d %d", "help2", 0, false, print2 },
    { "print1", "%d", "help1", 0, false, print1 },
    { "print2", "%d %d", "help2", 0, false, print2 },
    { "print1", "%d", "help1", 0, false, print1 },
    { "print2", "%d %d", "help2", 0, false, print2 },
    { "print1", "%d", "help1", 0, false, print1 },
    { "print2", "%d %d", "help2", 0, false, print2 },
    { "print1", "%d", "help1", 0, false, print1 },
    { "print2", "%d %d", "help2", 0, false, print2 },
    { "print1", "%d", "help1", 0, false, print1 },
    { "print2", "%d %d", "help2", 0, false, print2 },
    { "print3", "%s %d %b", "help3", 0, false, print3 },
    { "print4", "", "help4", 0, false, print0 },
    { "print5", "%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d", "help5", 0, false, print5 },

    //{ "print0", NULL, "help0", 0, false, print0 },
    { NULL, },
};

int main()
{
    cli_init(cmds);

    printf("-- always be dispatching\n");

    cli_dispatch("print0");

    cli_dispatch("print1 4");

    cli_dispatch("print2 7 8");

    struct timespec start, end;
#define NUM_ITERATIONS (20000000)

    clock_gettime(CLOCK_MONOTONIC_RAW, &start);
    for (int i = 0; i < NUM_ITERATIONS; i++)
    {
        //cli_dispatch("print4");
        cli_dispatch("print5 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20");
    }
    clock_gettime(CLOCK_MONOTONIC_RAW, &end);

    uint64_t duration = (1000000000UL * (end.tv_sec - start.tv_sec)) + (end.tv_nsec - start.tv_nsec);

    printf("%lu p3_call\n", p3_call);
    printf("%lu iterations took %lu nanoseconds\n", NUM_ITERATIONS, duration);
    printf("%.03f million items/sec\n", (double)NUM_ITERATIONS / duration * 1000L);

    return 0;
}
