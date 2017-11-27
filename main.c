#include <stdio.h>
#include <sys/time.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>

#define BUFFER_SIZE 32
#define READ_END     0
#define WRITE_END    1

struct timeval stop, start;

const char * generateTimeStamp() {
	char returnString[48];
	gettimeofday(&stop, NULL);
	int minutes = (stop.tv_sec - start.tv_sec) / 60;
	int seconds = (stop.tv_sec - start.tv_sec) % 60;
	int microseconds = (stop.tv_usec - start.tv_usec) / 100;
	printf("%ld:%02ld.%03ld", minutes, seconds, microseconds);
	snprintf(returnString, sizeof(returnString), "%ld:%02ld.%03ld\n", minutes, seconds, microseconds);
	return returnString;
}

// Generate a random number between nMin and nMax. Example nMin = 2, and nMax = 4 will
// randomly generate either 2 or 3 or 4
int RandomNumberGenerator(const int nMin, const int nMax) {
	return rand() % (nMax + 1 - nMin) + nMin;
}

int main(int argc, char **argv) {
	// sets initial time
	gettimeofday(&start, NULL);
    printf("hello");
	return 0;
}