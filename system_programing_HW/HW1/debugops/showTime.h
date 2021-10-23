#ifndef SHOW_TIME_H
#define SHOW_TIME_H
#include <sys/time.h>
#include <stdio.h>
struct timeval startTime;
struct timeval endTime;
void timecheck(int isItStart)
{
	double diff;
	if(isItStart)
		gettimeofday(&startTime, NULL);
	else
	{
		gettimeofday(&endTime, NULL);
		diff = endTime.tv_sec + endTime.tv_usec / 1000000.0 -
			startTime.tv_sec - startTime.tv_usec / 1000000.0;
		printf("Flow Time: %f sec\n", diff);
	}
}
#endif
