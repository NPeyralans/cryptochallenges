#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* Notice that each base64 character is indexed by the value it is encoding.
 * I.e. base64Table[000111] = base64Table[7] = "H".
 */
const char base64Table[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

// First we will convert our input string to raw bytes
unsigned char hexCharToBytes(char c) {
	if ('0' <= c && c <= '9') return c - '0';
	if ('a' <= c && c <= 'f') return c - 'a' + 10;
	if ('A' <= c && c <= 'F') return c - 'A' + 10;
	return 0;
}

unsigned char* hexToBytes(const char* hex, size_t* out_len){
	size_t hex_length = strlen(hex);

	*out_len = hex_length / 2;
	unsigned char* bytes = malloc(*out_len);
	if (!bytes) {
		fprintf(stderr, "Failed to allocate memory for raw bytes!");
		return NULL;
	}

	for (size_t i = 0; i < *out_len; i++){
		unsigned char high = hexCharToBytes(hex[2*i]);
		unsigned char low = hexCharToBytes(hex[2*i + 1]);
		bytes[i] = (high << 4) | low;
	}
	return bytes;
}

// This function will take the input data and convert to base64
char* base64_encode(const unsigned char* data, size_t input_length){
	/* Take the input string "49276d20" as an example.
	 * It has length of 8 bytes, or 64 bits.
	 * This is represented on the byte level as 34 39 32 37 36 64 32 30.
	 * Each hex character is 4 bits, with each couplet one byte, for 64 bits.
	 * Base64 encoding takes every six bits and encodes it as a character.
	 * If we took the 8 bytes and divided by 3, we would get 2; so this doesn't work.
	 * We take the input length, add 2 to it, then divide by 3 to ensure that 
	 * if the input is not divisible by 3, it will not terminate before there 
	 * is a base64 encoding due to lack of bits. 
	 * However, base64 is always in multiples of 4
	 * and will pad the end with "=" until that is achieved. 
	 * This is represented in base64 as NDkyNzZkMjA=, 12 characters.
	 */

	/* input_length = 8; 8 = 2 = 10. 10 / 3 = 3 (remainer discarded). 
	 * 4 * 3 = 12; the desired size of our output.
	 */

	size_t output_length = 4 * ((input_length + 2) / 3);
	//printf("output_length: %zu\n", output_length);

	// Add one for the null terminator 
	char* encoded = malloc(output_length + 1);
	if (!encoded) return NULL;

	/* Base64 encodes 3 bytes (24 bits) into 4 sextets
	 * i.e. every three bytes becomes 4 characters,
	 * so we will process the data in three byte chunks
	 */
	int i, j;
	for (i = 0, j = 0; i < input_length;){
		unsigned int octet_a = i < input_length ? data[i++] : 0;
		unsigned int octet_b = i < input_length ? data[i++] : 0;
		unsigned int octet_c = i < input_length ? data[i++] : 0;
	
		// printf("octet_a: %x\n", octet_a);
		// printf("octet_b: %x\n", octet_b);
		// printf("octet_c: %x\n", octet_c);
	
		/* Now we have three bytes, i.e. three octets. We first
	 	* need to combine it into one triple, and then we can 
	 	* decompose it into 4 sextets.
	 	* We use bit-shift << to combine the 3 bytes into one 
	 	* 24-bit int.
	 	*/
		unsigned int triple = (octet_a << 16) | (octet_b << 8) | octet_c;
		// printf("triple: %02x\n", triple);
	
		encoded[j++] = base64Table[(triple >> 18) & 0x3F];
		encoded[j++] = base64Table[(triple >> 12) & 0x3F];
		// If the input is short one byte, we pad with =
		encoded[j++] = (i > input_length + 1) ? '=' : base64Table[(triple >> 6) & 0x3F];
		// We will need two pads if short by two chars
		encoded[j++] = (i > input_length) ? '=' : base64Table[triple & 0x3F];
		
	}
	
	encoded[output_length] = '\0';
	return encoded;
}

int main(int argc, char* argv[]) {
	char* input = NULL;
	if (argc < 2) {
		fprintf(stderr, "Usage:\n");
		fprintf(stderr, " %s <string>\n", argv[0]);
		fprintf(stderr, " %s -f <filename>\n", argv[0]);
		return 1;
	}


	// If -f or --file is provided, then read the file for input
	if ((strcmp(argv[1], "-f") == 0) || (strcmp(argv[1], "--file") == 0)) {
		if (argc < 3) {
			fprintf(stderr, "Filename missing after -f\n");
			return 1;
		}

		const char *filename = argv[2];
		FILE *file = fopen(filename, "rb");

		if (!file) {
			fprintf(stderr, "Error opening file!\n");
			return 1;
		}

		// Determine file size to allocate memory
		fseek(file, 0, SEEK_END);
		long length = ftell(file);
		rewind(file);

		// Allocate memory for file contents
		input = malloc(length + 1);
		if (!input) {
			fprintf(stderr, "Memory allocation failed\n");
			fclose(file);
			return 1;
		}

		// Read file into buffer
		fread(input, 1, length, file);
		input[length] = '\0';
		fclose(file);

	} else {
		// If no file provided, we just read the string after
		input = argv[1];
	}
	
	size_t len = strlen(input);
	unsigned char* raw_input = hexToBytes(input, &len);	
	char* b64 = base64_encode(raw_input, len);

	printf("%s\n", b64);
	free(b64);

	if ((strcmp(argv[1], "-f") == 0) || (strcmp(argv[1], "--file") == 0)) {
		// Free the memory allocated
		free(input);
	}
	return 0;
}
