#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/wait.h>

#define MAX_HISTORY 100
#define MAX_STR_LEN 256

//check that a string contains only letters, return 1 if valid 0 if not
int letters_only(char *str) {

    int i = 0;
    while(str[i] != '\0') {
        if(!isalpha(str[i])) {
            return 0;
        }
        i++;
    }
    return 1;
}

//convert a string to uppercase in place
void to_uppercase(char *str) {

    int i = 0;
    while(str[i] != '\0') {
        str[i] = toupper(str[i]);
        i++;
    }
}

//convert a string to lowercase in place
void to_lowercase(char *str) {

    int i = 0;
    while(str[i] != '\0') {
        str[i] = tolower(str[i]);
        i++;
    }
}

//print all strings currently stored in the history
void show_history(char history[][MAX_STR_LEN], int history_count) {

    if(history_count == 0) {
        printf("History is empty.\n");
        return;
    }

    int i = 0;
    while(i < history_count) {
        printf("%d. %s\n", i + 1, history[i]);
        i++;
    }
}

//prompt the user to pick a string from history or enter a new one
//returns the chosen string into the out buffer
//returns 1 if a string was selected, 0 if the user cancelled
int pick_from_history_or_new(char history[][MAX_STR_LEN], int history_count, char *out) {

    char choice[8];
    printf("Use a string from history or enter a new one? (h/n): ");
    scanf("%s", choice);
    to_lowercase(choice);

    //if the user wants to use history show the list and let them pick
    if(strcmp(choice, "h") == 0) {

        if(history_count == 0) {
            printf("History is empty. Please enter a new string.\n");
        }
        else {
            show_history(history, history_count);
            printf("0. Enter a new string\n");
            printf("Choice: ");

            int selection;
            scanf("%d", &selection);

            //if they picked a valid history item copy it to out
            if(selection >= 1 && selection <= history_count) {
                strcpy(out, history[selection - 1]);
                return 1;
            }

            //if they picked 0 fall through to enter a new string below
            if(selection != 0) {
                printf("Invalid selection.\n");
                return 0;
            }
        }
    }

    //prompt the user to enter a new string
    printf("Enter string: ");
    scanf("%s", out);

    //validate that the string contains only letters
    if(!letters_only(out)) {
        printf("Error: input must contain letters only.\n");
        return 0;
    }

    return 1;
}

int main(int argc, char *argv[]) {

    //make sure a log file name was provided
    if(argc < 2) {
        fprintf(stderr, "Usage: %s <logfile>\n", argv[0]);
        return 1;
    }

    //pipes for the logger - driver writes logs, logger reads and writes to file
    int log_pipe[2];

    //pipes for the encryption program - two way communication
    int enc_in[2];   //driver writes commands, encryption reads them
    int enc_out[2];  //encryption writes results, driver reads them

    //pids for the two child processes
    pid_t logger_pid;
    pid_t enc_pid;

    //history array to store strings entered this session
    char history[MAX_HISTORY][MAX_STR_LEN];
    int history_count = 0;

    //create all pipes before forking so all processes can inherit them
    if(pipe(log_pipe) == -1) {
        perror("pipe failed for logger");
        exit(EXIT_FAILURE);
    }

    if(pipe(enc_in) == -1) {
        perror("pipe failed for encryption input");
        exit(EXIT_FAILURE);
    }

    if(pipe(enc_out) == -1) {
        perror("pipe failed for encryption output");
        exit(EXIT_FAILURE);
    }

    //fork the logger child process
    logger_pid = fork();

    if(logger_pid == -1) {
        perror("fork failed for logger");
        exit(EXIT_FAILURE);
    }

    if(logger_pid == 0) {

        //inside the logger child - redirect stdin to read from the log pipe
        dup2(log_pipe[0], STDIN_FILENO);

        //close all pipe ends in the child since dup2 already set up what we need
        close(log_pipe[0]);
        close(log_pipe[1]);
        close(enc_in[0]);
        close(enc_in[1]);
        close(enc_out[0]);
        close(enc_out[1]);

        //execute the logger program with the log file name as an argument
        execl("./logger", "logger", argv[1], NULL);

        //if exec fails something went wrong
        perror("execl failed for logger");
        exit(EXIT_FAILURE);
    }

    //fork the encryption child process
    enc_pid = fork();

    if(enc_pid == -1) {
        perror("fork failed for encryption");
        exit(EXIT_FAILURE);
    }

    if(enc_pid == 0) {

        //inside the encryption child - redirect stdin to read commands from driver
        dup2(enc_in[0], STDIN_FILENO);

        //redirect stdout to write results back to the driver
        dup2(enc_out[1], STDOUT_FILENO);

        //close all pipe ends in the child since dup2 already set up what we need
        close(enc_in[0]);
        close(enc_in[1]);
        close(enc_out[0]);
        close(enc_out[1]);
        close(log_pipe[0]);
        close(log_pipe[1]);

        //execute the encryption program
        execl("./encryption", "encryption", NULL);

        //if exec fails something went wrong
        perror("execl failed for encryption");
        exit(EXIT_FAILURE);
    }

    //in the parent - close the pipe ends we do not need
    close(log_pipe[0]);  //parent only writes to the logger not reads
    close(enc_in[0]);    //parent only writes commands to encryption not reads
    close(enc_out[1]);   //parent only reads results from encryption not writes

    //open file streams over the pipe file descriptors for easier reading and writing
    FILE *log_write = fdopen(log_pipe[1], "w");
    FILE *enc_write = fdopen(enc_in[1], "w");
    FILE *enc_read  = fdopen(enc_out[0], "r");

    if(log_write == NULL || enc_write == NULL || enc_read == NULL) {
        perror("fdopen failed");
        exit(EXIT_FAILURE);
    }

    //log that the driver has started
    fprintf(log_write, "START Driver started.\n");
    fflush(log_write);

    char command[64];
    char input[MAX_STR_LEN];
    char response[MAX_STR_LEN];

    //print the menu and start the main loop
    printf("\n=== Encryption Driver ===\n");
    printf("Commands: password, encrypt, decrypt, history, quit\n\n");

    while(1) {

        printf("> ");
        scanf("%s", command);
        to_lowercase(command);

        //handle the password command - set the passkey in the encryption program
        if(strcmp(command, "password") == 0) {

            //let the user pick from history or enter a new password
            char choice[8];
            int selected = 0;

            printf("Use a string from history or enter a new one? (h/n): ");
            scanf("%s", choice);
            to_lowercase(choice);

            if(strcmp(choice, "h") == 0 && history_count > 0) {

                show_history(history, history_count);
                printf("0. Enter a new string\n");
                printf("Choice: ");

                int selection;
                scanf("%d", &selection);

                if(selection >= 1 && selection <= history_count) {
                    strcpy(input, history[selection - 1]);
                    selected = 1;
                }
            }

            if(!selected) {
                printf("Enter password: ");
                scanf("%s", input);

                //validate only letters
                if(!letters_only(input)) {
                    printf("Error: password must contain letters only.\n");
                    continue;
                }
            }

            //convert to uppercase before sending to the encryption program
            to_uppercase(input);

            //send the PASSKEY command to the encryption program
            fprintf(enc_write, "PASSKEY %s\n", input);
            fflush(enc_write);

            //read the result back from the encryption program
            fgets(response, sizeof(response), enc_read);
            response[strcspn(response, "\n")] = '\0';

            printf("Password set.\n");

            //log the action but never log the password itself
            fprintf(log_write, "PASSWORD Password set.\n");
            fflush(log_write);
        }

        //handle the encrypt command - encrypt a string using the current passkey
        else if(strcmp(command, "encrypt") == 0) {

            //let the user pick from history or enter a new string to encrypt
            if(!pick_from_history_or_new(history, history_count, input)) {
                continue;
            }

            //save the input string to history before encrypting
            if(history_count < MAX_HISTORY) {
                strcpy(history[history_count], input);
                history_count++;
            }

            //convert to uppercase before sending to the encryption program
            to_uppercase(input);

            //send the ENCRYPT command to the encryption program
            fprintf(enc_write, "ENCRYPT %s\n", input);
            fflush(enc_write);

            //read the result back from the encryption program
            fgets(response, sizeof(response), enc_read);
            response[strcspn(response, "\n")] = '\0';

            //check if the response was an error
            if(strncmp(response, "ERROR", 5) == 0) {
                printf("%s\n", response);
                fprintf(log_write, "ENCRYPT %s\n", response);
                fflush(log_write);
                continue;
            }

            //extract the result from the response and print it
            char *result = response + 7;  //skip "RESULT "
            printf("Result: %s\n", result);

            //save the result to history
            if(history_count < MAX_HISTORY) {
                strcpy(history[history_count], result);
                history_count++;
            }

            //log the encrypt action and result
            fprintf(log_write, "ENCRYPT Result: %s\n", result);
            fflush(log_write);
        }

        //handle the decrypt command - decrypt a string using the current passkey
        else if(strcmp(command, "decrypt") == 0) {

            //let the user pick from history or enter a new string to decrypt
            if(!pick_from_history_or_new(history, history_count, input)) {
                continue;
            }

            //save the input string to history before decrypting
            if(history_count < MAX_HISTORY) {
                strcpy(history[history_count], input);
                history_count++;
            }

            //convert to uppercase before sending to the encryption program
            to_uppercase(input);

            //send the DECRYPT command to the encryption program
            fprintf(enc_write, "DECRYPT %s\n", input);
            fflush(enc_write);

            //read the result back from the encryption program
            fgets(response, sizeof(response), enc_read);
            response[strcspn(response, "\n")] = '\0';

            //check if the response was an error
            if(strncmp(response, "ERROR", 5) == 0) {
                printf("%s\n", response);
                fprintf(log_write, "DECRYPT %s\n", response);
                fflush(log_write);
                continue;
            }

            //extract the result from the response and print it
            char *result = response + 7;  //skip "RESULT "
            printf("Result: %s\n", result);

            //save the result to history
            if(history_count < MAX_HISTORY) {
                strcpy(history[history_count], result);
                history_count++;
            }

            //log the decrypt action and result
            fprintf(log_write, "DECRYPT Result: %s\n", result);
            fflush(log_write);
        }

        //handle the history command - print all strings stored this session
        else if(strcmp(command, "history") == 0) {
            show_history(history, history_count);
            fprintf(log_write, "HISTORY History displayed.\n");
            fflush(log_write);
        }

        //handle the quit command - shut down both child processes and exit
        else if(strcmp(command, "quit") == 0) {

            //send QUIT to the encryption program first
            fprintf(enc_write, "QUIT\n");
            fflush(enc_write);

            //log the exit BEFORE sending QUIT to the logger (QUIT is not logged)
            fprintf(log_write, "EXIT Driver exiting.\n");
            fflush(log_write);

            //now signal the logger to quit
            fprintf(log_write, "QUIT\n");
            fflush(log_write);

            //close all streams so the children receive EOF
            fclose(enc_write);
            fclose(enc_read);
            fclose(log_write);

            //wait for both child processes to finish before exiting
            waitpid(enc_pid, NULL, 0);
            waitpid(logger_pid, NULL, 0);

            printf("Goodbye.\n");
            break;
        }

        //if the command is not recognized tell the user
        else {
            printf("Unknown command. Commands: password, encrypt, decrypt, history, quit\n");
        }
    }

    return 0;
}
