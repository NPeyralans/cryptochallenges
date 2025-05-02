/* breakit.c */
#include "../programs/functions.h"

int main(int argc, char* argv[]){

	// File input
	if (argc < 2) {
		printf("Usage: %s <filename>\n", argv[0]);
		return 1;
	}

	unsigned char* ciphertext;
	int ciphertext_len = read_base64_file(argv[1], &ciphertext);
	if (ciphertext_len == -1){
		return -1;
	}

	decrypt_repeating_key_xor(ciphertext, ciphertext_len);

	free(ciphertext);

	return 0;
}
