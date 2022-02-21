#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "logger.h"

static int rand_range(int min, int max)
{
    return rand() % (max - min + 1) + min;
}

static void log_msg(Log *log, const char *proc, int level)
{
    char *msgs[] = {
        "Debug message",
        "Info message",
        "Warning message",
        "Error message"};

    log_write(log, proc, level, msgs[level]);
    printf("%s\n", msgs[level]);
}

int main(int argc, char **argv)
{
    srand(time(NULL));

    Log *log;
    log_init(&log);

    while (1)
    {
        int l = rand_range(0, 3);
        int t = rand_range(0, 9);
        log_msg(log, argv[0], l);
        struct timespec remaining, request = {0, t * 100000000};
        nanosleep(&request, &remaining);
    }

    log_free(log);
    return 0;
}
