#include "../programs/functions.h"

const char base64_table[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";

// Open file 
int read_file(const char* filename, char** output, size_t* size){
	FILE *fp = fopen(filename, "rb");
	if (!fp){
		perror("Failed to open file!\n");
		return -1;
	}

	fseek(fp, 0, SEEK_END);
	size_t file_size = ftell(fp);
	rewind(fp);

	char* buffer = malloc(file_size);

	if (!buffer){
		perror("Memory allocation failed!\n");
		fclose(fp);
		return -1;
	}

	size_t bytes_read = fread(buffer, 1, file_size, fp);
	fclose(fp);

	if (bytes_read != file_size){
		perror("Failed to read entire file!\n");
		free(buffer);
		return -1;
	}

	*output = buffer;
	if (size){
		*size = file_size;
	}

	return 0;

}

// Decode base 64 from input
int base64_decode(const char* input, unsigned char** output){

	int input_len = strlen(input);
	
	// Handle padding
	int padding = 0;
	if (input[input_len-1] == '='){
		padding++;
		if (input[input_len-2] == '='){
			padding++;
		}
	}

	int decoded_length = (input_len * 3) / 4 - padding;

	*output = (unsigned char*)malloc(decoded_length);
	if (*output == NULL){
		return -1;
	}

	int i = 0;
	int j = 0;
	int k = 0;

	unsigned char temp[4];
	while (i < input_len){
		for (k = 0; k < 4; k++){
			if (i < input_len){
				char c = input[i++];
				if (c == '='){
					temp[k] = 0;
				} else{
					temp[k] = strchr(base64_table, c) - base64_table;
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

// Score output to see if human readable
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

// Compute hamming distance between two bytes
int ham(const unsigned char* byte1, const unsigned char* byte2, size_t len){

	int counter = 0;
	for (size_t i = 0; i < len; i++){
		unsigned char byte = byte1[i] ^ byte2[i];
		while (byte){
			counter += byte & 1;
			byte >>= 1;
		}
	}

	return counter;

}

// Normalize hamming distance from length of text and keysize
float norm_ham(unsigned char* ciphertext, int keysize, int len){
	long total_dist = 0;
	int comparisons = 0;

	for (int i = 0; i < len - keysize; i += keysize){
		unsigned char* chunk1 = &ciphertext[i];
		unsigned char* chunk2 = &ciphertext[i + keysize];
		total_dist += ham(chunk1, chunk2, keysize);
		comparisons++;
	}

	float norm_dist = (float)total_dist / (comparisons * keysize);
	return norm_dist;
}


