#include "../programs/functions.h"

int main(int argc, char* argv[]){
	if (argc < 2){
		printf("Usage: %s <filename.txt>\n", argv[0]);
		return 1;
	}
	
	FILE *fp = fopen(argv[1], "r");
	if (fp == NULL){
		perror("Error opening file\n");
		return 1;
	}

	char line[MAX_LINE];

	int bestScore = -100000;
	decodedXOR bestResult;

	while (fgets(line, sizeof(line), fp)){
		line[strcspn(line, "\n")] = '\0';
		decodedXOR candidate = decodeXOR(line);
		if (candidate.score > bestScore){
			bestResult = candidate;
			bestScore = candidate.score;
		} 
	}

	printf("The line decoded is: %s\n", bestResult.cipher);
	printf("Deciphered message: %s\n", bestResult.decodedMessage);
	/* printf("With key: %c\n", bestResult.key); */
	/* printf("and score: %d\n", bestResult.score); */

	fclose(fp);
	
}
