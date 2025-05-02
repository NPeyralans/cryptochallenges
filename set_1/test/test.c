#include <stdio.h>
#include <stdint.h>

// Hamming Distance Function with Debugging
int hamming_distance(unsigned char *byte1, unsigned char *byte2, int len) {
    int dist = 0;
    for (int i = 0; i < len; i++) {
        unsigned char xor_result = byte1[i] ^ byte2[i]; // XOR to find differences
        printf("XOR result for byte %d: %02X\n", i, xor_result);  // Debug print for XOR result
        // Count number of 1s (set bits) in xor_result
        while (xor_result) {
            dist += xor_result & 1;
            xor_result >>= 1;
        }
    }
    return dist;
}

int main() {
    // Test Data 1 (Example: byte1 and byte2)
    unsigned char byte1[] = {0xAA, 0xBB};  // 10101010 10111011 (in binary)
    unsigned char byte2[] = {0x55, 0xCC};  // 01010101 11001100 (in binary)

    // Calculate the Hamming distance between byte1 and byte2
    int len = sizeof(byte1) / sizeof(byte1[0]); // Size of byte array (2 bytes in this case)
    int dist = hamming_distance(byte1, byte2, len);

    // Print the test data and the Hamming distance
    printf("Byte1: ");
    for (int i = 0; i < len; i++) {
        printf("%02X ", byte1[i]);
    }
    printf("\n");

    printf("Byte2: ");
    for (int i = 0; i < len; i++) {
        printf("%02X ", byte2[i]);
    }
    printf("\n");

    printf("Hamming distance between byte1 and byte2: %d\n", dist);

    return 0;
}#include <stdio.h>
#include <stdint.h>

// Hamming Distance Function with Debugging
int hamming_distance(unsigned char *byte1, unsigned char *byte2, int len) {
    int dist = 0;
    for (int i = 0; i < len; i++) {
        unsigned char xor_result = byte1[i] ^ byte2[i]; // XOR to find differences
        printf("XOR result for byte %d: %02X\n", i, xor_result);  // Debug print for XOR result
        // Count number of 1s (set bits) in xor_result
        while (xor_result) {
            dist += xor_result & 1;
            xor_result >>= 1;
        }
    }
    return dist;
}

int main() {
    // Test Data 1 (Example: byte1 and byte2)
    unsigned char byte1[] = {0xAA, 0xBB};  // 10101010 10111011 (in binary)
    unsigned char byte2[] = {0x55, 0xCC};  // 01010101 11001100 (in binary)

    // Calculate the Hamming distance between byte1 and byte2
    int len = sizeof(byte1) / sizeof(byte1[0]); // Size of byte array (2 bytes in this case)
    int dist = hamming_distance(byte1, byte2, len);

    // Print the test data and the Hamming distance
    printf("Byte1: ");
    for (int i = 0; i < len; i++) {
        printf("%02X ", byte1[i]);
    }
    printf("\n");

    printf("Byte2: ");
    for (int i = 0; i < len; i++) {
        printf("%02X ", byte2[i]);
    }
    printf("\n");

    printf("Hamming distance between byte1 and byte2: %d\n", dist);

    return 0;
}
