// functions.h
#ifndef FUNCTIONS_H 
#define FUNCTIONS_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_LINE 1024 

typedef struct {
	const unsigned char* cipher;
	unsigned char key;
	unsigned char* decodedMessage;
	int score;
} decodedXOR;

unsigned char hexCharToBytes(char c);
unsigned char* hexToBytes(const char* hex, size_t* out_len);
char* base64_encode(const unsigned char* data, size_t input_length);
char* bytesToHex(const unsigned char* bytes, size_t len);
char* xor(unsigned char* bytes1, unsigned char* bytes2, size_t len);
unsigned char* createKeyString(char c, size_t len);
int score_text(const unsigned char* text, size_t len);
decodedXOR decodeXOR(const char* input);

#endif
