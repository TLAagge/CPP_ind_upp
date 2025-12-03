#ifndef SAFEINPUT_H
#define SAFEINPUT_H

//Safe input function to prevent buffer overflow
//Reads a line of input with a maximum length
void safeInput(char* buffer, int maxLength);

//Reads an integer safely with validation
int safeInputInt(const char* prompt);

#endif
