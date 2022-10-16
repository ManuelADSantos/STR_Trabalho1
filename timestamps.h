#ifndef TIMESTAMPS
#define TIMESTAMPS "timestamps.h"

#include "time.h"
#include "stdio.h"

struct timespec sum_timestamp(struct timespec begin, struct timespec end);

struct timespec sub_timestamp(struct timespec begin, struct timespec end);

double time_between_timestamp(struct timespec begin, struct timespec end);

#endif