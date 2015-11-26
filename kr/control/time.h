#include <sys/time.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

struct timeval time_print();
void timeval_print(struct timeval *tv);
int timeval_subtract(struct timeval *result, struct timeval *t2, struct timeval *t1);
void time_diff_print(struct timeval *tvDiff, struct timeval *tvEnd, struct timeval *tvBegin);

