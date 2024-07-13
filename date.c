#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <time.h>

// Function to display the time described by STRING
void displayTime(const char *string) {
    struct tm tm_time;
    memset(&tm_time, 0, sizeof(struct tm));

    // Parse the time string
    if (strptime(string, "%Y-%m-%d %H:%M:%S", &tm_time) == NULL) {
        perror("Error parsing time string");
        exit(1); // Exit with an error code
    }

    // Convert the time to a timestamp
    time_t timestamp = mktime(&tm_time);

    // Display the formatted time
    char formattedTime[100];
    strftime(formattedTime, sizeof(formattedTime), "%c", localtime(&timestamp));
    printf("Time: %s\n", formattedTime);
}

int main(int argc, char *argv[]) {
    // Check the number of arguments
    if (argc < 2) {
        fprintf(stderr, "Usage: date [-d STRING] [-R] file_name\n");
        return 1; // Return an error code
    }

    char *string = NULL;
    int useRFC5322 = 0;

    // Parse options
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-d") == 0) {
            // Use the next argument as the time string
            if (i + 1 < argc) {
                string = argv[i + 1];
                i++; // Skip the next argument
            } else {
                fprintf(stderr, "Error: -d option requires a time string\n");
                return 1; // Return an error code
            }
        } else if (strcmp(argv[i], "-R") == 0) {
            useRFC5322 = 1;
        } else {
            // Assume the argument is a file name

            // Fork a child process
            pid_t pid = fork();

            if (pid == -1) {
                perror("Error forking process");
                return 1; // Return an error code
            }

            if (pid == 0) {
                // This is the child process

                struct stat file_stat;
                if (stat(argv[i], &file_stat) != 0) {
                    perror("Error getting file status");
                    exit(1); // Exit with an error code
                }

                if (useRFC5322) {
                    // Format date and time in RFC 5322 format
                    char dateStr[100];
                    strftime(dateStr, sizeof(dateStr), "%a, %d %b %Y %T %z", gmtime(&(file_stat.st_mtime)));
                    printf("%s\n", dateStr);
                } else if (string) {
                    // Display the time described by STRING
                    displayTime(string);
                } else {
                    // Display the last modified date and time
                    printf("Last modified: %s", ctime(&(file_stat.st_mtime)));
                }

                exit(0);
            } else {
                // This is the parent process

                // Wait for the child process to complete
                int status;
                waitpid(pid, &status, 0);

                if (!WIFEXITED(status)) {
                    fprintf(stderr, "Child process terminated abnormally\n");
                    return 1; // Return an error code
                }
            }
        }
    }

    return 0; // Return success code
}
