#include <stdio.h>
#include <string.h>
#include "SafeInput.h"

void safeInput(char* buffer, int maxLength) { //Safe input to prevent buffer overflow
    if (fgets(buffer, maxLength, stdin) != NULL) {
        //Remove newline character if present
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len - 1] == '\n') {
            buffer[len - 1] = '\0';
        }
        
        //Clear any remaining characters in input buffer
        if (len == maxLength - 1 && buffer[len - 1] != '\n') {
            int c;
            while ((c = getchar()) != '\n' && c != EOF);
        }
    } else {
        buffer[0] = '\0';
    }
}

int safeInputInt(const char* prompt) { //Reads an integer safely with validation
    char buffer[20];
    int value;
    int valid = 0;
     
    while (!valid) { //Loop until valid input is received
        printf("%s", prompt);
        safeInput(buffer, sizeof(buffer));
        
        if (sscanf(buffer, "%d", &value) == 1) {
            valid = 1;
        } else {
            printf("Invalid input. Please enter a number.\n");
        }
    }
    
    return value;
}
