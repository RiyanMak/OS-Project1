#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
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
    char exit[] = "EXIT";

    //loop to keep running to listen for log Actions and messages
    while(1){
        

    }



    //Always close the file at the end after write operations.
    fclose(file);

}
