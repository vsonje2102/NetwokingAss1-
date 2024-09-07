#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

void sendMessageMenu(int instance_count, char *sys_ids[], char *ports[], int system_number);

int main(int argc, char *argv[]) {
    if (argc != 4) {
        fprintf(stderr, "Usage: %s <file_name> <port_no> <instance_count>\n", argv[0]);
        return 1;
    }

    char *file_name = argv[1];
    char *port_no = argv[2];
    int instance_count = atoi(argv[3]);

    char *sys_ids[instance_count];
    char *ports[instance_count];

    for (int i = 0; i < instance_count; ++i) {
        sys_ids[i] = malloc(256);
        ports[i] = malloc(256);
        if (!sys_ids[i] || !ports[i]) {
            fprintf(stderr, "Memory allocation failed\n");
            return 1;
        }
    }

    FILE *file = fopen(file_name, "r");
    if (!file) {
        perror("fopen");
        return 1;
    }

    char line[256];
    int index = 0;
    while (fgets(line, sizeof(line), file) && index < instance_count) {
        sscanf(line, "%s %s", sys_ids[index], ports[index]);
        index++;
    }

    fclose(file);

    int system_number = -1;
    for (int i = 0; i < instance_count; ++i) {
        if (strcmp(port_no, ports[i]) == 0) {
            system_number = i + 1;
            break;
        }
    }
    if (system_number == -1) {
        fprintf(stderr, "System number not found for port: %s\n", port_no);
        return 1;
    }

    printf("System ID is: %s\n", sys_ids[system_number - 1]);
    printf("Port no is: %s\n", ports[system_number - 1]);

    pid_t pid = fork();
    if (pid == 0) {
        char *argsUr[] = {"./ur", port_no, NULL};
        execv(argsUr[0], argsUr);
        perror("execv failed");
        exit(EXIT_FAILURE);
    }

    int choice;
    while (1) {
        printf("===== Communication Menu =====\n");
        printf("System %d (Port: %s)\n", system_number, ports[system_number - 1]);
        printf("Select what to do:\n");
        printf("1) View Sent Messages\n");
        printf("2) View Received Messages\n");
        printf("3) Send Messages\n");
        printf("4) EXIT\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: {
                char fname[512];
                char ch;
                sprintf(fname, "sentmsg_%s.txt", ports[system_number - 1]);
                printf("%s\n", fname);
                FILE *fp = fopen(fname, "r");
                if (fp == NULL) {
                    perror("fopen");
                    break;
                }
                while ((ch = getc(fp)) != EOF) {
                    putchar(ch);
                }
                fclose(fp);
                break;
            }
            case 2: {
                char fname[512];
                char ch;
                sprintf(fname, "recvMessage_%s.txt", ports[system_number - 1]);
                printf("%s\n", fname);
                FILE *fp = fopen(fname, "r");
                if (fp == NULL) {
                    perror("fopen");
                    break;
                }
                while ((ch = getc(fp)) != EOF) {
                    putchar(ch);
                }
                fclose(fp);
                break;
            }
            case 3:
                sendMessageMenu(instance_count, sys_ids, ports, system_number);
                break;
            case 4:
                for (int i = 0; i < instance_count; ++i) {
                    char fname_sent[512];
                    char fname_recv[512];
                    sprintf(fname_sent, "sentmsg_%s.txt", ports[i]);
                    sprintf(fname_recv, "recvMessage_%s.txt", ports[i]);
                    remove(fname_sent);
                    remove(fname_recv);
                    free(sys_ids[i]);
                    free(ports[i]);
                }
                return 0;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }

    for (int i = 0; i < instance_count; ++i) {
        free(sys_ids[i]);
        free(ports[i]);
    }

    return 0;
}

void sendMessageMenu(int instance_count, char *sys_ids[], char *ports[], int system_number) {
    printf("===== Communication Menu =====\n");
    printf("System %d (Port: %s)\n", system_number, ports[system_number - 1]);
    printf("Select the system to send a message to:\n");
    for (int i = 1; i <= instance_count; ++i) {
        printf("%d. System %s (Port: %s)\n", i, sys_ids[i - 1], ports[i - 1]);
    }
    printf("0. Exit\n");
    printf("Enter your choice: ");

    int choice;
    scanf("%d", &choice);

    if (choice == 0) {
        for (int i = 0; i < instance_count; ++i) {
            char fname_sent[512];
            char fname_recv[512];
            sprintf(fname_sent, "sentmsg_%s.txt", ports[i]);
            sprintf(fname_recv, "recvMessage_%s.txt", ports[i]);
            remove(fname_sent);
            remove(fname_recv);
        }
        exit(0);
    }

    if (choice < 1 || choice > instance_count) {
        printf("Invalid choice. Please try again.\n");
        return;
    }

    char message[256];
    printf("Enter the message to be sent: ");
    scanf(" %[^\n]", message);

    char *argsUs[] = {"./us", ports[choice - 1], sys_ids[choice - 1], message, ports[system_number - 1], sys_ids[system_number - 1], NULL};
    pid_t pid = fork();
    if (pid == 0) {
        execv(argsUs[0], argsUs);
        perror("execv failed");
        exit(EXIT_FAILURE);
    } else {
        wait(NULL); // Wait for the child process to finish
    }
}

