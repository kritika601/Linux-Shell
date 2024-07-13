#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

// Function to count words in a file
int count_words(const char *filename, int ignoreNewline) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        exit(1); // Exit with an error code
    }

    int word_count = 0;
    int in_word = 0; // Flag to indicate if currently inside a word
    char c;

    while ((c = fgetc(file)) != EOF) {
        if (c == ' ' || (c == '\n' && !ignoreNewline)) {
            in_word = 0;
        } else {
            if (!in_word) {
                word_count++;
                in_word = 1;
            }
        }
    }

    fclose(file);

    return word_count;
}

int main(int argc, char *argv[]) {
    // Check the number of arguments
    if (argc < 2) {
        fprintf(stderr, "Usage: word [-n] [-d] file_name\n");
        return 1; // Return an error code
    }

    int ignoreNewline = 0;
    int calculate_difference = 0; // Initialize to 0
    int word_count = 0;

    //Store the word counts in a n array
    int word_counts[argc];

    // Parse options
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-n") == 0) {
            ignoreNewline = 1;
        } else if (strcmp(argv[i], "-d") == 0) {
            calculate_difference = 1; // Set to 1 when -d is encountered
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

                // Calculate word count in the file
                int count = count_words(argv[i], ignoreNewline);

                // Write the word count to the parent process using the exit status
                exit(count);
            } else {
                // This is the parent process

                // Wait for the child process to complete
                int status;
                waitpid(pid, &status, 0);

                if (WIFEXITED(status)) {
                    int exit_status = WEXITSTATUS(status);
		    word_counts[i] = exit_status;

                    // Subtract the word count if calculate_difference is 1
                    //if (calculate_difference) {
                      //  word_count -= exit_status;
                    //} else {
                      //  word_count += exit_status;
                    //}
                
		} else {
                    fprintf(stderr, "Child process terminated abnormally\n");
                    return 1; // Return an error code
                }
            }
        }
    }

    // Calculate the difference if -d option is used
    if (calculate_difference) {
	if (argc >= 4) {
	   word_count = word_counts[argc - 2] - word_counts[argc - 1];
	}else {
	  fprintf(stderr, "Error: -d option requires two file names\n");
	  return 1;
	}
    } else {
	// Calculate the total word count
	for (int i=1; i < argc; i++) {
	    word_count += word_counts[i];
	}
    }

    printf("Total words are: %d\n", word_count);

    return 0; // Return success code
}
