#include <stdio.h>
#include "logger.h"
#include "encryption.h"

int main(int argc, char *argv[]) {

    if (argc < 2){
        fprintf(stderr, "Usage: %s <logfile>\n", argv[0]);
        return 1;
    }

    //send the file name to the logger function
    logger(argv[1]);

    return 0;
    
}