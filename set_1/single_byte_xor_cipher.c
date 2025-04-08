#include "functions.h"

int main(int argc, char* argv[]){
	// First we will take the input and translate it to bytes
	if (argc < 2){
		fprintf(stderr, "Usage: <string>\n");
		return 1;
	}

	const char* input = argv[1];
	size_t len;
	
	decodedXOR result = decodeXOR(input);

	printf("Decrypted message: %s\n", result.decodedMessage);
	printf("Decrypted with key: %c\n", result.key);

    return 0;
}
