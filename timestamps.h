#ifndef TIMESTAMPS
#define TIMESTAMPS "timestamps.h"

#include "time.h"
#include "stdio.h"

struct timespec sum_timestamp(struct timespec a, struct timespec b);

struct timespec sub_timestamp(struct timespec a, struct timespec b);

double time_between_timestamp(struct timespec begin, struct timespec end);

#endif