/* breakit.c */
#include "../programs/functions.h"

/* This program will try every keysize from 2 to 40 to determine
 * variable sized repeated key xors and then decode the input file
 */
int main(int argc, char* argv[]){

	// Let's test our hamming Function
	unsigned char* test_string1 = "this is a test";
	unsigned char* test_string2 = "wokka wokka!!!";
	int dist = hamming_distance(test_string1, test_string2, 14);
	printf("Testing hamming function...\n");
	printf("The distance between 'this is a test' and 'wokka wokka!!!' is %d\n", dist);
	if (dist == 37) {
		printf("Hamming function test passed!\n");
	} else {
		printf("Hamming function failed!\n");
		return -1;
	}

	printf("\n\n");

	// File input
	if (argc < 2) {
		printf("Usage: %s <filename>\n", argv[0]);
		return 1;
	}

	// Convert input to string in memory
	FILE *fp = fopen(argv[1], "r");
	if (!fp){
		perror("Failed to open file\n");
		return -1;
	}

	fseek(fp, 0, SEEK_END);
	long length = ftell(fp);
	rewind(fp);

	char* input = (char*)malloc(length+1);
	if (!input){
		perror("Memory allocation failed!\n");
		fclose(fp);
		return -1;
	}

	int bytes_read = fread(input, 1, length, fp);
	if (bytes_read != length){
		perror("Failed to read entire file!\n");
		free(input);
		fclose(fp);
		return -1;
	}

	fclose(fp);

	printf("Input string: \n");
	printf("%s\n\n", input);
	
	// Decode the input so we can work on the bytes
	unsigned char* decoded_input;
	int decoded_input_len = base64_decode(input, &decoded_input);
	printf("Decoding base64 input for XOR...\n\n\n");
	/* printf("Decoded input (in hex):\n"); */
	/* for (int i = 0; i < decoded_input_len; i++){ */
	/* 	printf("%02x ", decoded_input[i]); */
	/* } */

	printf("Decoded input length: %d\n", decoded_input_len);

	// Array to store each keysize's total_distance
	float total_normalized_distances[39];

	// We will iterate over the possible key sizes frtm 2 to 40
	for (int keysize = 2; keysize < 41; keysize++){

		// Hamming distance after comparing all successive bytes
		long total_distance = 0;
		int number_comparisons = 0;
		int max_index = decoded_input_len - (2*keysize) + 1;

		/* Now we will divide the input into $KEYSIZE segments and find the distance between them */
		for (int i = 0; i < max_index; i++){
			unsigned char chunk1[keysize];
			unsigned char chunk2[keysize];
		
			memcpy(chunk1, &decoded_input[i], keysize);
			memcpy(chunk2, &decoded_input[i + keysize], keysize);

			int dist = hamming_distance(chunk1, chunk2, keysize);
			total_distance += dist;

			number_comparisons++;
		}

		float ave_dist = (float)total_distance / (float)number_comparisons;
		float norm_dist = ave_dist / keysize;
		
/* 		printf("Dividing total_distance (%d) by number_comparisons (%d)...\n", total_distance, number_comparisons); */
/* 		printf("Dividing ave_dist (%f) by keysize (%d)...\n", ave_dist, keysize); */
/* 		printf("Normalized distance for keysize (%d): %f\n", keysize, norm_dist); */
		
		total_normalized_distances[keysize-2] = norm_dist;
		printf("Keysize %d: Normalized Distance = %f\n", keysize, total_normalized_distances[keysize - 2]);

	} /* End loop from 2 to 40 */

	return 0;
}
