#include "timestamps.h"
#include "time.h"
#include "stdio.h"

int main()
{
    struct timespec test_start, test_end, test_result;

    if (clock_gettime(CLOCK_REALTIME, &test_start) != -1)
    {
        printf("start_sec = %ld || start_nsec = %ld\n\n", test_start.tv_sec, test_start.tv_nsec);
    }

    for (int i = 0; i < 10; i++)
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
}

struct timespec sum_timestamp(struct timespec a, struct timespec b)
{
    struct timespec result;
    result.tv_sec = a.tv_sec + b.tv_sec;
    result.tv_nsec = a.tv_nsec + b.tv_nsec;
    return result;
}

struct timespec sub_timestamp(struct timespec a, struct timespec b)
{
    struct timespec result;
    result.tv_sec = b.tv_sec - a.tv_sec;
    result.tv_nsec = b.tv_nsec - a.tv_nsec;
    return result;
}