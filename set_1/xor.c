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

	unsigned char* result = malloc(len1 + 1);

	if (result == NULL) {
		perror("malloc");
		free(bytes1);
		free(bytes2);
		return 1;
	}

	for (size_t i = 0; i < len1; i++){
		result[i] = bytes1[i] ^ bytes2[i];
	}

	// I need to convert the result of the XOR operation back into a string
	char* output = bytesToHex(result, len1);
	printf("%s\n", output);

	free(bytes1);
	free(bytes2);
	free(result);
	//printf("bytes: %s\n", bytes);
	//printf("bytes: %c\n", bytes);
	//printf("bytes: %02x\n", bytes);
	return 0;
}
