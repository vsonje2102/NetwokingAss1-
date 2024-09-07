#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>

#define MAX_LINE_LENGTH 256
#define COMMAND_BUFFER_SIZE 1024  // Buffer size for the command

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <file>\n", argv[0]);
        return 1;
    }

    const char *file_name = argv[1];
    FILE *file = fopen(file_name, "r");
    if (!file) {
        perror("fopen");
	printf("%s\n",argv[1]);
        return 1;
    }

    char line[MAX_LINE_LENGTH];
    int lineCnt = 0;

    // Count the number of lines in the file to determine the number of instances
    while (fgets(line, sizeof(line), file)) {
        lineCnt++;
    }
    rewind(file);

    for (int i = 0; i < lineCnt; ++i) {
        pid_t pid = fork();
        if (pid == -1) {
            perror("fork");
            fclose(file);
            return 1;
        }

        if (pid == 0) {
            // Child process
            char sys_id[MAX_LINE_LENGTH];
            char port_no[MAX_LINE_LENGTH];

            // Move the file pointer to the correct line for this process
            fseek(file, 0, SEEK_SET);
            for (int j = 0; j <= i; ++j) {
                fgets(line, sizeof(line), file);
            }
            sscanf(line, "%s %s", sys_id, port_no);

            // Prepare the command and its arguments for gnome-terminal
            char command[COMMAND_BUFFER_SIZE];
            snprintf(command, sizeof(command),"gnome-terminal -- bash -c './brain %s %s %d; exec zsh'",file_name, port_no, lineCnt);

            system(command); // Launch the gnome-terminal with the command
            fclose(file);    // Close the file in the child process
            exit(0);
        }
    }

    // Parent process waits for all child processes to finish
    for (int i = 0; i < lineCnt; ++i) {
        wait(NULL);
    }

    fclose(file);
    return 0;
}

