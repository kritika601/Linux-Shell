#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

// Function to create a directory
int create_directory(const char *dir_name, int remove_existing, int verbose) {

    // Checking if the directory already exists
    if (access(dir_name, F_OK) == 0) {
        // if directory exists
        if (remove_existing) {
            // Remove the existing directory
            if (rmdir(dir_name) != 0) {
                perror("Error removing existing directory");
                return 1; // Return an error code
            }
            if (verbose) {
                printf("Removed existing directory: %s\n", dir_name);
            }
        } else {
            fprintf(stderr, "Error: Directory already exists: %s\n", dir_name);
            return 1; // Return an error code
        }
    }

    // Fork a child process
    pid_t pid = fork();

    if (pid == -1) {
        perror("Error forking process");
        return 1; // Return an error code
    }

    if (pid == 0) {
        // This is the child process

        // Build the command to create the directory
        char mkdir_cmd[256];
        snprintf(mkdir_cmd, sizeof(mkdir_cmd), "mkdir %s", dir_name);

        // Execute the mkdir command
        int exec_result = execlp("sh", "sh", "-c", mkdir_cmd, (char *)NULL);

        if (exec_result == -1) {
            perror("Error executing mkdir");
            exit(1); // Exit with an error code
        }
    } else {
        // This is the parent process

        // Wait for the child process to complete
        int status;
        waitpid(pid, &status, 0);

        if (WIFEXITED(status)) {
            int exit_status = WEXITSTATUS(status);
            if (exit_status != 0) {
                fprintf(stderr, "Error creating directory\n");
                return 1; // Return an error code
            }
        } else {
            fprintf(stderr, "Child process terminated\n");
            return 1; // Return an error code
        }

        if (verbose) {
            printf("Created directory: %s\n", dir_name);
        }
    }

    return 0;
}

int main(int argc, char *argv[]) {
    int remove_existing = 0;
    int verbose = 0;
    char *dir_name = NULL;

    // Parse command line arguments and options
    if (argc < 2) {
        fprintf(stderr, "Usage: dir [-r] [-v] dir_name\n");
        return 1; // Return an error code
    }

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-r") == 0) {
            remove_existing = 1;
        } else if (strcmp(argv[i], "-v") == 0) {
            verbose = 1;
        } else {
            // Assuming that the argument is the directory name
            dir_name = argv[i];
        }
    }

    if (dir_name == NULL) {
        fprintf(stderr, "Error: Missing directory name\n");
        return 1; // Return an error code
    }

    int result = create_directory(dir_name, remove_existing, verbose);

    if (result != 0) {
        return result; 
    }

    return 0; 
}
