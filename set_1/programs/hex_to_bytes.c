// hex_to_bytes.c
#include "functions.h"

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
	/*printf("raw_input printed:\n");
	for (size_t i = 0; i < len; i++){
		printf("%02x", raw_input[i]);
	}
	printf("\n");
	*/
	char* b64 = base64_encode(raw_input, len);

	printf("%s\n", b64);
	free(b64);

	if ((strcmp(argv[1], "-f") == 0) || (strcmp(argv[1], "--file") == 0)) {
		// Free the memory allocated
		free(input);
	}
	return 0;
}
