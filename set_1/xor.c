// xor.c
/* functions in hexToBytes(const char* hex, size_t* out_len) */
#include "functions.h"

int main(int argc, char* argv[]) {
	
	// Verify two inputs have been provided
	if (argc < 3) {
		fprintf(stderr, "There must be two arguments provided to XOR!");
		return 1;
	}

	char* input1 = argv[1];
	char* input2 = argv[2];
	size_t len1 = strlen(input1);
	size_t len2 = strlen(input2);

	// Make sure each input is of equal length
	if (!len1==len2){
		fprintf(stderr, "Provided inputs must be of equal lengths!");
		return 1;
	}

	//printf("len1: %zu\n", len1);
	//printf("len2: %zu\n", len2);
	
	unsigned char* bytes1 = hexToBytes(input1, &len1);
	unsigned char* bytes2 = hexToBytes(input2, &len2);

	// printf("hexToBytes(input1): %02x\n", bytes1);
	// printf("hexToBytes(input2): %02x\n", bytes2);

	char* return_string = xor(bytes1, bytes2, len1);
	printf("Result: %s\n", return_string);

	free(bytes1);
	free(bytes2);

	return 0;
}
