/* functions.h */
#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>

int base64_decode(const char* encoded, unsigned char** decoded);
char* xor(unsigned char* byte1, unsigned char* byte2, size_t len);
int score_text(const unsigned char* text, size_t len);
int ham(const unsigned char* byte1, const unsigned char* byte2, size_t len);
float norm_ham(unsigned char* ctext, int keysize, int len);
int read_file(const char* filename, char** output, size_t* size);

#endif
