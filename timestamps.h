#ifndef TIMESTAMPS
#define TIMESTAMPS "timestamps.h"

struct timespec sub_timestamp(struct timespec begin, struct timespec end);

double time_between_timestamp(struct timespec begin, struct timespec end);

#endif