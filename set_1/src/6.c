#include "../programs/functions.h"

int main(int argc, char* argv[]){
	
	if (argc < 2){
		printf("Usage: %s <filename>\n", argv[0]);
	}

	// Read file into memory
	char *base64_input = NULL;
	int success = read_file(argv[1], base64_input);
	if (success != 0) perror("Failed to read file!\n");

	// Decode base64
	unsigned char** ciphertext = NULL;
	int len = base64_decode(base64_input, ciphertext);

	// Iterate over possible key sizes
	
	int best_k = 0;
	float best_dist = FLT_MAX;
	
	for (int k = 2; k < 41; k++){
		int total_dist = 0;
		int compares = 0;

		int max = len - ( 2 * k ) + 1;
		for (int i = 0; i < max; i++){
			unsigned char chunk1[k];
			unsigned char chunk2[k];
			
			memcpy(chunk1, &ciphertext[i], k);
			memcpy(chunk2, &ciphertext[i + k], k);

			int dist = ham(chunk1, chunk2, k);
			total_dist += dist;
			compares++;
		}

		// Normalize the value
		float norm_dist = (float)total_dist / (k * compares);

		// Save best key and update best_dist
		if (norm_dist < best_dist){
			best_dist = norm_dist;
			best_k = k;
		}
	}

	printf("The best normalized distance is %f\n", best_dist);
	printf("And the best keysize is %d\n", best_k);

	return 0;
}
