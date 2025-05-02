// functions.h
#ifndef FUNCTIONS_H 
#define FUNCTIONS_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>

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
int base64_decode(const char* input, unsigned char** output);
char* bytesToHex(const unsigned char* bytes, size_t len);
char* xor(unsigned char* bytes1, unsigned char* bytes2, size_t len);
unsigned char* createKeyString(char c, size_t len);
int score_text(const unsigned char* text, size_t len);
decodedXOR decodeXOR(const char* input);
char* file_to_plaintext(const char* filename);
int hamming_distance(const unsigned char* byte1, const unsigned char* byte2, size_t length);
float normalize_hamming_distance(unsigned char* ciphertext, int keysize, int legnth);
void xor_single_byte(unsigned char* ciphertext, unsigned char* result, int leg, unsigned char key);
void decrypt_repeating_key_xor(unsigned char* ciphertext, int length);
int read_base64_file(const char* filename, unsigned char** ciphertext);

#endif
