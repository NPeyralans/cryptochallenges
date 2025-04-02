// functions.h
#ifndef FUNCTIONS_H 
#define FUNCTIONS_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

unsigned char hexCharToBytes(char c);
unsigned char* hexToBytes(const char* hex, size_t* out_len);
char* base64_encode(const unsigned char* data, size_t input_length);
char* bytesToHex(const unsigned char* bytes, size_t len);

#endif
