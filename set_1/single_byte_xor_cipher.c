#include "functions.h"

int main(int argc, char* argv[]){
	// First we will take the input and translate it to bytes
	if (argc < 2){
		fprintf(stderr, "Usage: <string>\n");
	}

	const char* input = argv[1];
	size_t len;
	/* printf("Input: %s\n", input); */
	/* printf("Length: %zu\n", len); */
	
	unsigned char* inputBytes = hexToBytes(input, &len);
	/* printf("inputBytes: %zu\n", inputBytes); */
	
	int bestScore = -1000000;
	unsigned char bestKey = 0;
	unsigned char* bestCandidate = NULL;

	// Then we xor each byte with keys 0-256
	for (unsigned int key = 0; key < 256; key++){
		unsigned char* deciphered = malloc(len + 1);
		if (!deciphered){
			perror("malloc");
			exit(EXIT_FAILURE);
		}

		for (size_t i = 0; i < len; i++){
			deciphered[i] = inputBytes[i] ^ key;
		}
		deciphered[len] = '\0';

		int score = score_text(deciphered, len);
		if (score > bestScore){
			bestScore = score;
			bestKey = (unsigned char) key;
			if (bestCandidate){
				free(bestCandidate);
			}
			bestCandidate = deciphered;
		} else {
			free(deciphered);
		}
	}

	printf("Decrypted message: %s\n", bestCandidate);
	printf("Decrypted with key: %c\n", bestKey);

	free(bestCandidate);
	free(inputBytes);

    return 0;
}
