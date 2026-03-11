#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <time.h>
#include "logger.h"

void logger(char *logfileName) {
    
    //Open a logger file with the filename to write to 
    FILE *file = fopen(logfileName, "w+");

    //if file doesnt exist return errors
    if(file == NULL){

        perror("Error opening a file");
        exit(EXIT_FAILURE);
    
    }
    //Lets get the first non-whitespace characters
    char *input = (char*)malloc(350 * sizeof(char));

    //if malloc fails exit the program rather than crashing
    if(input == NULL){
        perror("Malloc failed");
        exit(EXIT_FAILURE);
    }

    char quit[] = "QUIT";
    char Action[100];
    char Message[250];

    //loop to keep running to listen for log Actions and messages
    while(1){
       scanf("%[^\n]%*c", input);

       //loop through the input string to find the first word
       int i = 0;

       while(1){
        char currentChar = input[i];
        if(currentChar == ' ' || currentChar == '\0'){

            //put everything else in the Message array
            int k = 0;
            for (int j = i + 1; j < strlen(input); j++){
                Message[k] = input[j];
                k++;
            }
            Message[k] = '\0';

            //after that break
            break;
        }
        else{
            Action[i] = currentChar;
            i++;
        }
       };
    Action[i] = '\0';

        //we will compare the strings lexiographically, quit without logging
        if(strcmp(Action, quit) == 0){
            break;
        };

    //now we will start writing to the log file
    char ActionString[105];
    snprintf(ActionString, sizeof(ActionString), "[%s]", Action);

    time_t now;
    struct tm *timeinfo;
    char buffer[20];

    time(&now);
    timeinfo = localtime(&now);
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M", timeinfo);

    fprintf(file, "%s %s %s\n", buffer, ActionString, Message);
    };

    free(input);
    fclose(file);


}
