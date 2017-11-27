
#include <stdio.h>
#include <pthread.h>
#include <sys/time.h>
#include <unistd.h>

struct timeval stop, start;
int running = 1;

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

void * programTracker() {
    gettimeofday(&stop, NULL);
    int seconds = stop.tv_sec - start.tv_sec;
    while (seconds <= 5) {
        printf("%s\n",generateTimeStamp());
        sleep(1);
        seconds = stop.tv_sec - start.tv_sec;
    }
    return NULL;
}

int main(int argc, char **argv) {
    gettimeofday(&start, NULL);
    pthread_t tids[12];
    pthread_create(&tids[0], NULL, programTracker, NULL);
    pthread_join(tids[0], NULL);
    return 0;
}