#include <stdio.h>
#include <sys/time.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <fcntl.h>
#include <sys/time.h>
#include <sys/ioctl.h>

#define BUFFER_SIZE 128
#define READ_END     0
#define WRITE_END    1

// Generate a random number between nMin and nMax. Example nMin = 2, and nMax = 4 will
// randomly generate either 2 or 3 or 4
int RandomNumberGenerator(const int nMin, const int nMax) {
    return rand() % (nMax + 1 - nMin) + nMin;
}

int main(int argc, char **argv) {
    struct timeval stop, start;
    // sets initial time
    gettimeofday(&start, NULL);

    char write_msg[BUFFER_SIZE];

    pid_t pid1, pid2, pid3, pid4, pid5;
    int fd[5][2];  // 5 pipes with 2 channel (input and output) file

    // Create the pipes (all 5).
    if (pipe(fd[0]) == -1 || pipe(fd[1]) == -1 || pipe(fd[2]) == -1 || pipe(fd[3]) == -1 || pipe(fd[4]) == -1) {
        printf(stderr, "pipe() failed");
        return 1;
    }

    fd_set inputs, inputfds;
    FD_ZERO(&inputs);

    pid1 = fork();
    if (pid1 == 0) {
        // close input side
        close(fd[0][READ_END]);
        // CHILD PROCESS.
        struct timeval stop;
        gettimeofday(&stop, NULL);
        int timestampSec = stop.tv_sec - start.tv_sec;
        int timestampUSec = stop.tv_usec - start.tv_usec;
        int messageCount = 1;
        while (timestampSec <= 30) {
            int minutes = (timestampSec) / 60;
            int seconds = (timestampSec) % 60;
            int microseconds = abs((timestampUSec) / 100);
            snprintf(write_msg, BUFFER_SIZE, "%ld:%02ld.%03d: Child 1 message %d",
                     minutes, seconds, microseconds, messageCount);
            write(fd[0][WRITE_END], write_msg, BUFFER_SIZE);
            sleep(RandomNumberGenerator(0, 2));
            messageCount++;
            gettimeofday(&stop, NULL);
            timestampSec = stop.tv_sec - start.tv_sec;
            timestampUSec = stop.tv_usec - start.tv_usec;
        }
        close(fd[0][WRITE_END]);
        exit(0);
    } else {
        FD_SET(fd[0][READ_END], &inputs);
        close(fd[0][WRITE_END]);
        pid2 = fork();
        if (pid2 == 0) {
            // close input side
            close(fd[1][READ_END]);
            // CHILD PROCESS.
            struct timeval stop;
            gettimeofday(&stop, NULL);
            int timestampSec = stop.tv_sec - start.tv_sec;
            int timestampUSec = stop.tv_usec - start.tv_usec;
            int messageCount = 1;
            while (timestampSec <= 30) {
                int minutes = (timestampSec) / 60;
                int seconds = (timestampSec) % 60;
                int microseconds = abs((timestampUSec) / 100);
                snprintf(write_msg, BUFFER_SIZE, "%ld:%02ld.%03d: Child 2 message %d",
                         minutes, seconds, microseconds, messageCount);
                write(fd[1][WRITE_END], write_msg, BUFFER_SIZE);
                sleep(RandomNumberGenerator(0, 2));
                messageCount++;
                gettimeofday(&stop, NULL);
                timestampSec = stop.tv_sec - start.tv_sec;
                timestampUSec = stop.tv_usec - start.tv_usec;
            }
            close(fd[1][WRITE_END]);
            exit(0);
        } else {
            FD_SET(fd[1][READ_END], &inputs);
            close(fd[1][WRITE_END]);
            pid3 = fork();
            if (pid3 == 0) {
                // close input side
                close(fd[2][READ_END]);
                // CHILD PROCESS.
                struct timeval stop;
                gettimeofday(&stop, NULL);
                int timestampSec = stop.tv_sec - start.tv_sec;
                int timestampUSec = stop.tv_usec - start.tv_usec;
                int messageCount = 1;
                while (timestampSec <= 30) {
                    int minutes = (timestampSec) / 60;
                    int seconds = (timestampSec) % 60;
                    int microseconds = abs((timestampUSec) / 100);
                    snprintf(write_msg, BUFFER_SIZE, "%ld:%02ld.%03d: Child 3 message %d",
                             minutes, seconds, microseconds, messageCount);
                    write(fd[2][WRITE_END], write_msg, BUFFER_SIZE);
                    sleep(RandomNumberGenerator(0, 2));
                    messageCount++;
                    gettimeofday(&stop, NULL);
                    timestampSec = stop.tv_sec - start.tv_sec;
                    timestampUSec = stop.tv_usec - start.tv_usec;
                }
                close(fd[2][WRITE_END]);
                exit(0);
            } else {
                FD_SET(fd[2][READ_END], &inputs);
                close(fd[2][WRITE_END]);
                pid4 = fork();
                if (pid4 == 0) {
                    // close input side
                    close(fd[3][READ_END]);
                    // CHILD PROCESS.
                    struct timeval stop;
                    gettimeofday(&stop, NULL);
                    int timestampSec = stop.tv_sec - start.tv_sec;
                    int timestampUSec = stop.tv_usec - start.tv_usec;
                    int messageCount = 1;
                    while (timestampSec <= 30) {
                        int minutes = (timestampSec) / 60;
                        int seconds = (timestampSec) % 60;
                        int microseconds = abs((timestampUSec) / 100);
                        snprintf(write_msg, BUFFER_SIZE, "%ld:%02ld.%03d: Child 4 message %d",
                                 minutes, seconds, microseconds, messageCount);
                        write(fd[3][WRITE_END], write_msg, BUFFER_SIZE);
                        sleep(RandomNumberGenerator(0, 2));
                        messageCount++;
                        gettimeofday(&stop, NULL);
                        timestampSec = stop.tv_sec - start.tv_sec;
                        timestampUSec = stop.tv_usec - start.tv_usec;
                    }
                    close(fd[3][WRITE_END]);
                    exit(0);
                } else {
                    FD_SET(fd[3][READ_END], &inputs);
                    close(fd[3][WRITE_END]);
                    // special input child
                    pid5 = fork();
                    if (pid5 == 0) {
                        // close input side
                        close(fd[4][READ_END]);
                        char buffer[128];
                        int result, nread;

                        fd_set inputs, inputfds; // sets of file descriptors
                        struct timeval timeout;

                        FD_ZERO(&inputs);        // initialize inputs to the empty set
                        FD_SET(0, &inputs);    // set file descriptor 0 (stdin)

                        //  Wait for input on stdin for a maximum of 2.5 seconds.
                        // CHILD PROCESS.
                        struct timeval stop;
                        gettimeofday(&stop, NULL);
                        int timestampSec = stop.tv_sec - start.tv_sec;
                        int timestampUSec = stop.tv_usec - start.tv_usec;
                        int minutes = (timestampSec) / 60;
                        int seconds = (timestampSec) % 60;
                        int microseconds = abs((timestampUSec) / 100);
                        printf("%ld:%02ld.%03d: Type a message and press enter: ", minutes, seconds, microseconds);
                        fflush(stdout);
                        while (timestampSec <= 30) {
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
                                    gettimeofday(&stop, NULL);
                                    timestampSec = stop.tv_sec - start.tv_sec;
                                    timestampUSec = stop.tv_usec - start.tv_usec;
                                    minutes = (timestampSec) / 60;
                                    seconds = (timestampSec) % 60;
                                    microseconds = abs((timestampUSec) / 100);
                                    printf("\n%ld:%02ld.%03d: Type a message and press enter: ", minutes, seconds, microseconds);
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
                                        gettimeofday(&stop, NULL);
                                        timestampSec = stop.tv_sec - start.tv_sec;
                                        timestampUSec = stop.tv_usec - start.tv_usec;
                                        minutes = (timestampSec) / 60;
                                        seconds = (timestampSec) % 60;
                                        microseconds = abs((timestampUSec) / 100);
                                        snprintf(write_msg, BUFFER_SIZE, "%ld:%02ld.%03d: Child 5 message: %s",
                                                 minutes, seconds, microseconds, buffer);
                                        write(fd[4][WRITE_END], write_msg, BUFFER_SIZE);
                                    }
                                    break;
                            }
                            gettimeofday(&stop, NULL);
                            timestampSec = stop.tv_sec - start.tv_sec;
                        }
                        close(fd[4][WRITE_END]);
                        exit(0);
                    }
                    else {
                        FD_SET(fd[4][READ_END], &inputs);
                        close(fd[4][WRITE_END]);
                    }
                }
            }
        }

        // We are in parent process
        struct timeval stop;
        char read_msg[BUFFER_SIZE];

        FILE *f = fopen("output.txt", "w");
        gettimeofday(&stop, NULL);
        int timestampSec = stop.tv_sec - start.tv_sec;
        struct timeval timeout;
        timeout.tv_sec = 2;
        timeout.tv_usec = 500000;
        int result;
        while (timestampSec <= 30) {
            inputfds = inputs;
            result = select(FD_SETSIZE, &inputfds, NULL, NULL, &timeout);

            switch (result) {

                case 0: {
                    fflush(stdout);
                    break;
                }

                default:
                    if (FD_ISSET(fd[0][READ_END], &inputfds)) {
                        if (read(fd[0][READ_END], read_msg, BUFFER_SIZE) > 0) {
                            gettimeofday(&stop, NULL);
                            timestampSec = stop.tv_sec - start.tv_sec;
                            int timestampUSec = stop.tv_usec - start.tv_usec;
                            int minutes = (timestampSec) / 60;
                            int seconds = (timestampSec) % 60;
                            int microseconds = abs((timestampUSec) / 100);
                            fprintf(f, "Received at: %ld:%02ld.%03d - Message: %s\n", minutes, seconds, microseconds, read_msg);
                        }
                    }
                    if (FD_ISSET(fd[1][READ_END], &inputfds)) {
                        if (read(fd[1][READ_END], read_msg, BUFFER_SIZE) > 0) {
                            gettimeofday(&stop, NULL);
                            timestampSec = stop.tv_sec - start.tv_sec;
                            int timestampUSec = stop.tv_usec - start.tv_usec;
                            int minutes = (timestampSec) / 60;
                            int seconds = (timestampSec) % 60;
                            int microseconds = abs((timestampUSec) / 100);
                            fprintf(f, "Received at: %ld:%02ld.%03d - Message: %s\n", minutes, seconds, microseconds, read_msg);
                        }
                    }
                    if (FD_ISSET(fd[2][READ_END], &inputfds)) {
                        if (read(fd[2][READ_END], read_msg, BUFFER_SIZE) > 0) {
                            gettimeofday(&stop, NULL);
                            timestampSec = stop.tv_sec - start.tv_sec;
                            int timestampUSec = stop.tv_usec - start.tv_usec;
                            int minutes = (timestampSec) / 60;
                            int seconds = (timestampSec) % 60;
                            int microseconds = abs((timestampUSec) / 100);
                            fprintf(f, "Received at: %ld:%02ld.%03d - Message: %s\n", minutes, seconds, microseconds, read_msg);
                        }
                    }
                    if (FD_ISSET(fd[3][READ_END], &inputfds)) {
                        if (read(fd[3][READ_END], read_msg, BUFFER_SIZE) > 0) {
                            gettimeofday(&stop, NULL);
                            timestampSec = stop.tv_sec - start.tv_sec;
                            int timestampUSec = stop.tv_usec - start.tv_usec;
                            int minutes = (timestampSec) / 60;
                            int seconds = (timestampSec) % 60;
                            int microseconds = abs((timestampUSec) / 100);
                            fprintf(f, "Received at: %ld:%02ld.%03d - Message: %s\n", minutes, seconds, microseconds, read_msg);
                        }
                    }
                    if (FD_ISSET(fd[4][READ_END], &inputfds)) {
                        if (read(fd[4][READ_END], read_msg, BUFFER_SIZE) > 0) {
                            gettimeofday(&stop, NULL);
                            timestampSec = stop.tv_sec - start.tv_sec;
                            int timestampUSec = stop.tv_usec - start.tv_usec;
                            int minutes = (timestampSec) / 60;
                            int seconds = (timestampSec) % 60;
                            int microseconds = abs((timestampUSec) / 100);
                            fprintf(f, "Received at: %ld:%02ld.%03d - Message: %s", minutes, seconds, microseconds, read_msg);
                        }
                    }
                    break;
            }
            gettimeofday(&stop, NULL);
            timestampSec = stop.tv_sec - start.tv_sec;
        }

        close(fd[0][READ_END]);
        close(fd[1][READ_END]);
        close(fd[2][READ_END]);
        close(fd[3][READ_END]);
        close(fd[4][READ_END]);
        fclose(f);
    }
    return 0;
}