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

// Encode an input string in base64 
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

// Decode base64, return length of output
// Input pointer to char** for decoded ouput
int base64_decode(const char* input, unsigned char** output){
	int input_len = strlen(input);
	
	// base64 has either one or two paddings that need to be accounted for
	int padding = 0;
	if (input[input_len-1] == '='){
		padding++;
		if (input[input_len-2] == '='){
			padding++;
		}
	}

	// Since each base64 char encodes 6 bits and each ACII char is 8 bits
	// Then the ouput text will always be 3/4 the input subtracting the padding
	int decoded_length = (input_len * 3) / 4 - padding;

	*output = (unsigned char*)malloc(decoded_length);
	if (*output == NULL){
		return -1;
	}

	int i = 0;
	int j = 0;
	int k = 0;

	// Because 4 bytes of base64 encodes 3 ASCII characters, we will operate
	// on 4 bytes of base64 at a time
	unsigned char temp[4];
	while (i < input_len){
		for (k = 0; k < 4; k++){
			// This account for <4 from end of input
			if (i < input_len){
				char c = input[i++];
				if (c == '='){
					temp[k] = 0;
				} else{
					// This locates the char in the base64 table
					temp[k] = strchr(base64Table, c) - base64Table;
				}
			} else {
				temp[k] = 0;
			}
		}	
	
		(*output)[j++] = (temp[0] << 2) | (temp[1] >> 4);
		if (j < decoded_length){
			(*output)[j++] = (temp[1] << 4) | (temp[2] >> 2);
		}
		if (j < decoded_length){
			(*output)[j++] = (temp[2] << 6) | temp[3];
		}
	}

	return decoded_length;	

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

// This takes a file as input and stores it in memory as raw bytes
int read_base64_file(const char* filename, unsigned char** ciphertext){
	FILE *fp = fopen(filename, "rb");
	printf("Opening file: %s\n", filename);
	
	if (!fp) {
		perror("Failed to open file\n");
		return -1;
	}
	
	fseek(fp, 0, SEEK_END);
	long file_size = ftell(fp);
	rewind(fp);

	char* base64_data = (char*)malloc(file_size+1);
	if (!base64_data){
		perror("Memory allocation failed!\n");
		fclose(fp);
		return -1;
	}

	size_t bytes_read = fread(base64_data, 1, file_size, fp);
	base64_data[bytes_read] = '\0';
	fclose(fp);

	printf("Base 64 data read into memory:\n");
	printf("\n%s\n", base64_data);

	int ciphertext_len = base64_decode(base64_data, ciphertext);
	free(base64_data);

	printf("Decoding base64...\n\n");
	printf("Ciphertext has been stored in 'ciphertext'\n");
	printf("ciphertext has length %d\n\n", ciphertext_len);

	return ciphertext_len;
}

int hamming_distance(const unsigned char* byte1, const unsigned char* byte2, size_t length){
	int counter = 0;

	for (size_t i = 0; i < length; i++){
		unsigned char byte = byte1[i] ^ byte2[i];
		while (byte){
			counter += byte & 1;
			byte >>= 1;
		}
	}

	return counter;

}

float normalize_hamming_distance(unsigned char* ciphertext, int keysize, int length){
	long total_distance = 0;
	int number_comparisons = 0;

	for (int i = 0; i < length - keysize; i += keysize){
		unsigned char* chunk1 = &ciphertext[i];
		unsigned char* chunk2 = &ciphertext[i + keysize];
		total_distance += hamming_distance(chunk1, chunk2, keysize);
		number_comparisons++;
	}

	printf("Calculating normalized hamming distance...\n");
	float norm_dist = (float)total_distance / (number_comparisons * keysize);
	printf("Normalized distance = %f\n\n", norm_dist);
	return norm_dist;
}

void xor_single_byte(unsigned char* ciphertext, unsigned char* result, int len, unsigned char key){
	for (int i = 0; i < len; i++){
		result[i] = ciphertext[i] ^ key;
	}
}

void decrypt_repeating_key_xor(unsigned char* ciphertext, int length){
	int best_keysize = 0;
	float best_normalized_dist = FLT_MAX;

	printf("Iterating through possible keysizes...\n\n");

	for (int keysize = 2; keysize < 41; keysize++){
		float norm_dist = normalize_hamming_distance(ciphertext, keysize, length);
		if (norm_dist < best_normalized_dist){
			best_normalized_dist = norm_dist;
			printf("A new best distance was found!\n\n");
			best_keysize = keysize;
			printf("Current best keysize = %d\n", best_keysize);
		}
	}


	printf("Best keysize: %d\n", best_keysize);

	unsigned char* transposed = malloc(best_keysize * (length / best_keysize) * sizeof(unsigned char));
	unsigned char* block = malloc(best_keysize * sizeof(unsigned char));

	for (int i = 0; i < best_keysize; i++){
		for (int j = 0; j < length / best_keysize; j++){
			transposed[i * (length / best_keysize) + j] = ciphertext[i + j * best_keysize];
		}
	}

	unsigned char key[best_keysize];

	for (int i = 0; i < best_keysize; i++){
		unsigned char best_byte = 0;
		float best_score = -FLT_MAX;

		for (unsigned char candidate = 0; candidate < 256; candidate++){
			xor_single_byte(&transposed[i * (length / best_keysize)], block, length / best_keysize, candidate);
			float score = score_text(block, length / best_keysize);
			if ( score > best_score ){
				best_score = score;
				best_byte = candidate;
				printf("Best score is: %f\n", best_score);
			}
		}
		/* printf("key[%d]: %02x", key[i], best_byte); */
		key[i] = best_byte;
		printf("Key byte %d: %c (0x%02x)\n", i, best_byte, best_byte);
	}

	unsigned char* decrypted = malloc(length * sizeof(unsigned char));
	for (int i = 0; i < length; i++){
		decrypted[i] = ciphertext[i] ^ key[i % best_keysize];
	}

	printf("\nDecrypted text:\n");
	for (int i = 0; i < length; i++){
		printf("%c",  decrypted[i]);
	}
	printf("\n");

	free(transposed);
	free(block);
	free(decrypted);
}


