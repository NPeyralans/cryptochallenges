// functions.c
#include "functions.h"

/* Notice that each base64 character is indexed by the value it is encoding.
 * I.e. base64Table[000111] = base64Table[7] = "H".
 */
const char base64Table[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

// First we will convert our input string to raw bytes
unsigned char hexCharToBytes(char c) {
	// Here we are using subtracting the ascii representation to get numeric value of char
	if ('0' <= c && c <= '9') return c - '0';
	if ('a' <= c && c <= 'f') return c - 'a' + 10;
	// Account for input being capital vs lowercase
	if ('A' <= c && c <= 'F') return c - 'A' + 10;
	return 0;
}

unsigned char* hexToBytes(const char* hex, size_t* out_len){
	size_t hex_length = strlen(hex);

	/* printf("Length of line in hexToBytes: %zu\n", hex_length); */

	if (hex_length % 2 != 0){
		fprintf(stderr, "The input hex string must contain an even number of bytes!\n");
	}

	*out_len = hex_length / 2;
	unsigned char* bytes = malloc(*out_len);
	if (!bytes) {
		fprintf(stderr, "Failed to allocate memory for raw bytes!");
		return NULL;
	}

	// Iterate over every character input of hex and add to an array of bytes
	for (size_t i = 0; i < *out_len; i++){
		// This takes the first nibble input
		unsigned char high = hexCharToBytes(hex[2*i]);
		// This takes the second nibble
		unsigned char low = hexCharToBytes(hex[2*i + 1]);
		// Now we combine the two nibbles extracted into one byte
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

// Convert raw bytes to a string showing its Hex
char* bytesToHex(const unsigned char* bytes, size_t len) {
	char* hexStr = malloc(len * 2 + 1); // +1 for null terminator
	if (!hexStr) {
		perror("malloc");
		exit(1);
	}
	for (size_t i = 0; i < len; i++){
		sprintf(hexStr + i * 2, "%02x", bytes[i]);
	}

	hexStr[len * 2] = '\0';
	return hexStr;
}

// Score the candidate plaintext based on a simple heuristic.
// Letters and spaces get bonus points; non-printable characters get penalized.
int score_text(const unsigned char *text, size_t len) {
    int score = 0;
    for (size_t i = 0; i < len; i++) {
        unsigned char c = text[i];
        if (c >= 'a' && c <= 'z') {
            score += 2;
        } else if (c >= 'A' && c <= 'Z') {
            score += 2;
        } else if (c == ' ') {
            score += 3;
        } else if (c >= 32 && c <= 126) { // Other printable characters.
            score += 1;
        } else {
            score -= 5; // Penalty for non-printable characters.
        }
    }
    return score;
}

decodedXOR decodeXOR(const char* inputString){
	decodedXOR result;
	result.cipher = inputString;
	size_t len;
	unsigned char* inputBytes = hexToBytes(inputString, &len);
	int bestScore = -100000;
	unsigned char bestKey = 0;
	unsigned char* bestCandidate = NULL;

	for (unsigned int key = 0; key < 256; key++){
		unsigned char* candidate = malloc(len+1);
		if (!candidate){
			perror("malloc");
			exit(EXIT_FAILURE);
		}

		for (size_t i = 0; i < len; i++){
			candidate[i] = inputBytes[i] ^ key;
		}
		candidate[len] = '\0';

		int score = score_text(candidate, len);
		if (score > bestScore){
			bestScore = score;
			bestKey = (unsigned char) key;
			if (bestCandidate) free(bestCandidate);
			bestCandidate = candidate;
		} else {
			free(candidate);
		}
	}
	result.key = bestKey;
	result.decodedMessage = bestCandidate;
	result.score = bestScore;
	return result;
}

char* file_to_plaintext(const char* filename){
	// Make sure file is opened in binary form "b"
	FILE *fp = fopen(filename, "rb");
	if (!fp) {
		perror("Failed to open file\n");
		return NULL;
	}
	
	fseek(fp, 0, SEEK_END);
	long length = ftell(fp);
	rewind(fp);

	unsigned char* buffer = malloc(length+1);
	if (!buffer){
		perror("Memory allocation failed!\n");
		fclose(fp);
		return NULL;
	}

	// fread(void *ptr, size_t size, size_t count, FILE *stream)
	// This takes a pointer to where the data read will be saved
	// the size of each item to be read in bytes
	// The number of items to be read
	// And the file stream to read from
	size_t bytesRead = fread(buffer, 1, length, fp);
	if (bytesRead != length){
		perror("Failed to read the entire file");
		free(buffer);
		fclose(fp);
		return NULL;
	}
	/* for (long i = 0; i < length; i++){ */
	/* 	printf("%02x", buffer[i]); */	
	/* } */
	/* printf("\n"); */

	fclose(fp);
	return buffer;

	// And the file stream to read from
	/* size_t bytesRead = fread(buffer, 1, length, fp); */
}
