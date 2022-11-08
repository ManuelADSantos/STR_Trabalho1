#include "timestamps.h"
#include "time.h"
#include "stdio.h"

/*int main()
{
    struct timespec test_start, test_end, test_result;

    if (clock_gettime(CLOCK_REALTIME, &test_start) != -1)
    {
        printf("start_sec = %ld || start_nsec = %ld\n\n", test_start.tv_sec, test_start.tv_nsec);
    }

    for (int i = 0; i < 1000; i++)
    {
    }

    if (clock_gettime(CLOCK_REALTIME, &test_end) != -1)
    {
        printf("end_sec = %ld || end_nsec = %ld\n\n", test_end.tv_sec, test_end.tv_nsec);
    }

    // Test sum_timestamp
    test_result = sum_timestamp(test_start, test_end);
    printf("result_sum_sec = %ld || result_sum_nsec = %ld\n\n", test_result.tv_sec, test_result.tv_nsec);

    // Test sub_timestamp
    test_result = sub_timestamp(test_start, test_end);
    printf("result_sub_sec = %ld || result_sub_nsec = %ld\n\n", test_result.tv_sec, test_result.tv_nsec);

    return 0;
}*/

struct timespec sub_timestamp(struct timespec begin, struct timespec end)
{
    struct timespec result;

    result.tv_sec = end.tv_sec - begin.tv_sec;
    result.tv_nsec = end.tv_nsec - begin.tv_nsec;

    if (result.tv_nsec < 0)
    {
        result.tv_sec -= 1;
        result.tv_nsec += 1e9;
    }

    return result;
}

struct timespec sum_timestamp(struct timespec begin, struct timespec end)
{
    struct timespec result;

    result.tv_sec = end.tv_sec + begin.tv_sec;
    result.tv_nsec = end.tv_nsec + begin.tv_nsec;

    if (result.tv_nsec > 1e9)
    {
        result.tv_sec += 1;
        result.tv_nsec -= 1e9;
    }

    return result;
}

double time_between_timestamp(struct timespec begin, struct timespec end)
{
    struct timespec calc;
    calc = sub_timestamp(begin, end);
    double result = (calc.tv_sec) * 1e3 + (calc.tv_nsec) / 1e6;

    return result;
}