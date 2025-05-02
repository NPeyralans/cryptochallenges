#include "../programs/functions.h"

int main(int argc, char* argv[]){
	// Make sure arguments are passed
	if (argc < 3){
		printf("Usage: %s <filename.txt> -k <key>\n", argv[0]);
		return 1;
	}

	// Extract key from command line
	char* key = NULL;
	for (int i = 1; i < argc; i++){
		if (strcmp(argv[i], "-k") == 0 && i + 1 < argc){
			key = argv[i+1];
		}
	}

	// If no key, return error
	if (key) {
		/* printf("Key: %s\n", key); */
	} else{
		fprintf(stderr, "Usage: %s -k <key>\n", argv[0]);
		return 1;
	}

	char* filename = argv[1];
	/* printf("Filename for ciphertext: %s\n", filename); */
	
	// We will read our file into a string of bytes to be xored
	char* plaintext = file_to_plaintext(filename);
	/* printf("The plaintext string to be xored: \n%s\n", plaintext); */

	// Now we need to iterate through each byte of plaintext and xor with key
	size_t key_size = strlen((char*)key);
	/* printf("The size of key passed is %zu\n", key_size); */
	size_t plaintext_length = strlen((char*)plaintext);
	/* printf("The size of plaintext passed is %zu\n", plaintext_length); */
	unsigned char* ciphertext = malloc(plaintext_length * sizeof(unsigned char)); 

	for (size_t i = 0; i < plaintext_length; i++){
		ciphertext[i] = plaintext[i] ^ key[i % key_size];		
	}
	size_t ciphertext_length = strlen((char*)ciphertext);
	/* printf("The size of ciphertext passed is %zu\n", ciphertext_length); */

	printf("Ciphertext:\n");
	for (size_t i = 0; i < ciphertext_length - 1; i++){
		printf("%02x", ciphertext[i]);
	}
	/* printf("\n"); */
	free(ciphertext);
	return 0;

}

