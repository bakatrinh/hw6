#include <stdio.h>
#include <sys/time.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <fcntl.h>
#include <sys/time.h>
#include <sys/ioctl.h>

#define BUFFER_SIZE 32
#define READ_END     0
#define WRITE_END    1

void getInput() {
    char buffer[128];
    int result, nread;

    fd_set inputs, inputfds; // sets of file descriptors
    struct timeval timeout;

    FD_ZERO(&inputs);        // initialize inputs to the empty set
    FD_SET(0, &inputs);    // set file descriptor 0 (stdin)

    //  Wait for input on stdin for a maximum of 2.5 seconds.
    for (;;) {
        inputfds = inputs;

        // 2.5 seconds.
        timeout.tv_sec = 2;
        timeout.tv_usec = 500000;

        // Get select() results.
        result = select(FD_SETSIZE, &inputfds, (fd_set *) 0, (fd_set *) 0,
                        &timeout);

        // Check the results.
        //   No input:  the program loops again.
        //   Got input: print what was typed, then terminate.
        //   Error:     terminate.
        switch (result) {
            // timeout w/o input
            case 0:
                printf("@");
                fflush(stdout);
                break;

                // error
            case -1:
                perror("select");
                exit(1);
                break;

                // If, during the wait, we have some action on the file descriptor,
                // we read the input on stdin and echo it whenever an
                // <end of line> character is received, until that input is Ctrl-D.
            default:
                // Got input
                if (FD_ISSET(0, &inputfds)) {
                    ioctl(0, FIONREAD, &nread);

                    if (nread == 0) {
                        printf("Keyboard input done.\n");
                        exit(0);
                    }

                    nread = read(0, buffer, nread);
                    buffer[nread] = 0;
                    printf("Read %d characters from the keyboard: %s", nread,
                           buffer);
                }
                break;
        }
    }
}

int calculateTimestampSec(struct timeval start, struct timeval stop) {
    return stop.tv_sec - start.tv_sec;
}

int calculateTimestampUSec(struct timeval start, struct timeval stop) {
    return stop.tv_usec - start.tv_usec;
}

const char *generateTimeStamp(int timestampSec, int timestampUSec) {
    char returnString[48];
    int minutes = (timestampSec) / 60;
    int seconds = (timestampSec) % 60;
    int microseconds = (timestampUSec) / 100;
    //printf("%ld:%02ld.%03ld", minutes, seconds, microseconds);
    snprintf(returnString, sizeof(returnString), "%ld:%02ld.%03ld", minutes, seconds, microseconds);
    return returnString;
}

// Generate a random number between nMin and nMax. Example nMin = 2, and nMax = 4 will
// randomly generate either 2 or 3 or 4
int RandomNumberGenerator(const int nMin, const int nMax) {
    return rand() % (nMax + 1 - nMin) + nMin;
}

int main(int argc, char **argv) {

    char write_msg[BUFFER_SIZE] = "You're my child process!";
    char read_msg[BUFFER_SIZE];

    pid_t pid1, pid2, pid3, pid4, pid5;
    int fd[2];  // file descriptors for the pipe
    char readbuffer[80];

    // Create the pipe.
    if (pipe(fd) == -1) {
        fprintf(stderr, "pipe() failed");
        return 1;
    }
//
//    // Fork a child process.
//    pid1 = fork();
//    if (pid1 > 0) {
//        // PARENT PROCESS.
//
//        // Close the unused READ end of the pipe.
//        close(fd[READ_END]);
//
//        write(fd[WRITE_END], write_msg, strlen(write_msg) + 1);
//        printf("Parent (%d): Wrote: '%s' to the pipe.\n", pid1, write_msg);
//
//        // Close the WRITE end of the pipe.
//        close(fd[WRITE_END]);
//    } else if (pid1 == 0) {
//        // CHILD PROCESS.
//        // Close the unused WRITE end of the pipe.
//        close(fd[WRITE_END]);
//
//        read(fd[READ_END], read_msg, BUFFER_SIZE);
//        printf("Child (%d): Read: '%s' from the pipe.\n", pid1,
//               read_msg);
//
//        // Close the READ end of the pipe.
//        close(fd[READ_END]);
//    } else {
//        fprintf(stderr, "fork() failed");
//        return 1;
//    }
    pid1 = fork();
    if (pid1 == 0) {
        close(fd[0]);
        // CHILD PROCESS.
        struct timeval stop, start;
        char messageString[48];
        // sets initial time
        gettimeofday(&start, NULL);

        gettimeofday(&stop, NULL);
        int timestampSec = stop.tv_sec - start.tv_sec;
        int timestampUSec = stop.tv_usec - start.tv_usec;
        int messageCount = 1;
        while (timestampSec <= 30) {
            int minutes = (timestampSec) / 60;
            int seconds = (timestampSec) % 60;
            int microseconds = (timestampUSec) / 100;
            snprintf(messageString, sizeof(messageString), "%ld:%02ld.%03ld: Child 1 message %d\n", minutes, seconds, microseconds, messageCount);
            write(fd[WRITE_END], messageString, (strlen(messageString)+1));
            sleep(RandomNumberGenerator(0, 2));
            messageCount++;
            gettimeofday(&stop, NULL);
            timestampSec = stop.tv_sec - start.tv_sec;
            timestampUSec = stop.tv_usec - start.tv_usec;
        }
        exit(0);
    }
    else {
        close(fd[1]);
        while (read(fd[READ_END], readbuffer, sizeof(readbuffer)) != READ_END) {
            printf("%s", readbuffer);
        }
    }

    return 0;
}