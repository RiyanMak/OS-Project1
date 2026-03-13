#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//store the current passkey and whether one has been set
char passkey[256] = "";
int has_passkey = 0;

//encrypt the plaintext using a Vigenere cipher with the given key
//both plaintext and key are expected to be uppercase letters only
void vigenere_encrypt(char *plaintext, char *key, char *ciphertext) {

    int key_len = strlen(key);
    int j = 0;
    int i = 0;

    //loop through each character of the plaintext and shift it
    while(plaintext[i] != '\0') {
        char k = key[j % key_len] - 'A';
        ciphertext[i] = ((plaintext[i] - 'A' + k) % 26) + 'A';
        i++;
        j++;
    }
    ciphertext[i] = '\0';
}

//decrypt the ciphertext using a Vigenere cipher with the given key
//both ciphertext and key are expected to be uppercase letters only
void vigenere_decrypt(char *ciphertext, char *key, char *plaintext) {

    int key_len = strlen(key);
    int j = 0;
    int i = 0;

    //loop through each character of the ciphertext and reverse the shift
    while(ciphertext[i] != '\0') {
        char k = key[j % key_len] - 'A';
        plaintext[i] = ((ciphertext[i] - 'A' - k + 26) % 26) + 'A';
        i++;
        j++;
    }
    plaintext[i] = '\0';
}

int main(void) {

    //allocate a buffer to hold each line of input
    char *input = (char*)malloc(512 * sizeof(char));

    //if malloc fails exit the program rather than crashing
    if(input == NULL) {
        perror("Malloc failed");
        exit(EXIT_FAILURE);
    }

    char command[64];
    char argument[448];
    char result[448];
    char quit[] = "QUIT";

    //loop to keep running and listen for commands from stdin
    while(1) {
        scanf("%[^\n]%*c", input);

        //loop through the input to extract the first word as the command
        int i = 0;

        while(1) {
            char currentChar = input[i];
            if(currentChar == ' ' || currentChar == '\0') {

                //put everything after the command into the argument array
                int k = 0;
                for(int j = i + 1; j < (int)strlen(input); j++) {
                    argument[k] = input[j];
                    k++;
                }
                argument[k] = '\0';

                //after extracting the argument break out of the inner loop
                break;
            }
            else {
                command[i] = currentChar;
                i++;
            }
        }
        command[i] = '\0';

        //check if the command is QUIT and exit the loop if so
        if(strcmp(command, quit) == 0) {
            break;
        }

        //check if the command is PASSKEY and set the current passkey
        else if(strcmp(command, "PASSKEY") == 0) {
            strcpy(passkey, argument);
            has_passkey = 1;
            printf("RESULT\n");
            fflush(stdout);
        }

        //check if the command is ENCRYPT and encrypt the argument
        else if(strcmp(command, "ENCRYPT") == 0) {

            //if no passkey has been set output an error
            if(!has_passkey) {
                printf("ERROR Password not set\n");
                fflush(stdout);
            }
            else {
                vigenere_encrypt(argument, passkey, result);
                printf("RESULT %s\n", result);
                fflush(stdout);
            }
        }

        //check if the command is DECRYPT and decrypt the argument
        else if(strcmp(command, "DECRYPT") == 0) {

            //if no passkey has been set output an error
            if(!has_passkey) {
                printf("ERROR Password not set\n");
                fflush(stdout);
            }
            else {
                vigenere_decrypt(argument, passkey, result);
                printf("RESULT %s\n", result);
                fflush(stdout);
            }
        }

        //if the command is unrecognized output an error
        else {
            printf("ERROR Unknown command\n");
            fflush(stdout);
        }
    }

    free(input);

    return 0;
}
