# Linux-Shell

This program acts as a special unix system which has 3 commands for the C shell:
1. word command: The word command is a utility program that counts the number of
words in one or more text files. It supports the following options:
- -n: Ignores newline characters while counting words.
- -d: Calculates the difference in word counts between two text files.
If we don't specify any options, the program will count words in the provided text file.
The program uses fork, exec, and wait system calls to create child processes for each
file specified. Each child process calculates the word count for a single file, and the
parent process collects the results and calculates the total word count or difference.
2. dir command: The dir command is a Unix-like shell command that creates a directory
and changes the working directory to that directory. It also provides options to handle
existing directories and print verbose messages. It has the following options:
- -r: Remove an existing directory with the same name and create a new one.
- -v: Print a message for each step in the running of this command.
The program parses command line arguments to determine options and the directory
name. It checks if the directory already exists and handles it based on the -r option. It
forks a child process to execute the mkdir command to create the directory. The parent
process waits for the child process to complete and changes the working directory to the
newly created directory. Verbose messages are printed if the -v option is specified.
3. date command: This C program implements a simplified version of the date command in
a Unix-like shell. It takes a filename as an argument and returns the last modification
date and time of that file. It has the following options:
- -d: displays time described by STRING
- -R: outputs date and time in RFC 5322 format
The program first checks the command-line arguments. If not enough arguments are
provided or the file does not exist, it displays an error message and exits with an error
code.
It parses the command-line options and arguments to determine if the -d option and a
custom date string are provided.
If a custom date string is provided using the -d option, it attempts to parse this string into
a struct tm structure using strptime. If the parsing fails, it displays an error message and
exits with an error code.
It uses the stat function to retrieve the file's status, including its last modification time.
If a custom date string is provided, it replaces the modification time with the parsed
custom date and time.
It then converts the modified time (either the original modification time or the custom date
and time) into a human-readable string format using strftime.
If the -R option is chosen, it displays the date and time in RFC 5322 format.
Finally, it prints the date and time to the console.
