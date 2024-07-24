#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

// Permuted Choice 1 (PC-1) table: converts 64-bit key to two 28-bit halves
int PC1[56] = {
    57, 49, 41, 33, 25, 17, 9,
    1, 58, 50, 42, 34, 26, 18,
    10, 2, 59, 51, 43, 35, 27,
    19, 11, 3, 60, 52, 44, 36,
    63, 55, 47, 39, 31, 23, 15,
    7, 62, 54, 46, 38, 30, 22,
    14, 6, 61, 53, 45, 37, 29,
    21, 13, 5, 28, 20, 12, 4
};

// Left shift schedule: number of shifts per round
int leftShifts[16] = {
    1, 1, 2, 2, 2, 2, 2, 2,
    1, 2, 2, 2, 2, 2, 2, 1
};

// Permuted Choice 2 (PC-2) table: creates 48-bit subkey from 56-bit halves
int PC2[48] = {
    14, 17, 11, 24, 1, 5, 3, 28,
    15, 6, 21, 10, 23, 19, 12, 4,
    26, 8, 16, 7, 27, 20, 13, 2,
    41, 52, 31, 37, 47, 55, 30, 40,
    51, 45, 33, 48, 44, 49, 39, 56,
    34, 53, 46, 42, 50, 36, 29, 32
};

// Function to perform left circular shift
void leftCircularShift(uint32_t *half, int shifts) {
    *half = ((*half << shifts) | (*half >> (28 - shifts))) & 0x0FFFFFFF;
}

// Function to generate subkeys from the initial key
void generateSubkeys(uint64_t initialKey, uint64_t subkeys[16]) {
    uint32_t C = 0, D = 0;
    uint64_t permutedKey = 0;

    // Perform the initial permutation (PC-1) on the initial key
    for (int i = 0; i < 56; i++) {
        permutedKey <<= 1;
        permutedKey |= (initialKey >> (64 - PC1[i])) & 0x01;
    }

    // Split the permuted key into two 28-bit halves
    C = (permutedKey >> 28) & 0x0FFFFFFF;
    D = permutedKey & 0x0FFFFFFF;

    // Generate 16 subkeys
    for (int i = 0; i < 16; i++) {
        // Perform the left circular shifts according to the schedule
        leftCircularShift(&C, leftShifts[i]);
        leftCircularShift(&D, leftShifts[i]);

        // Combine C and D into a single 56-bit value
        uint64_t CD = ((uint64_t)C << 28) | D;

        // Perform the second permutation (PC-2) to generate the subkey
        uint64_t subkey = 0;
        for (int j = 0; j < 48; j++) {
            subkey <<= 1;
            subkey |= (CD >> (56 - PC2[j])) & 0x01;
        }

        subkeys[i] = subkey;
    }
}

// Function to print the subkeys
void printSubkeys(uint64_t subkeys[16]) {
    for (int i = 0; i < 16; i++) {
        printf("Subkey %2d: ", i + 1);
        for (int j = 0; j < 48; j++) {
            printf("%llu", (subkeys[i] >> (47 - j)) & 0x01);
            if ((j + 1) % 8 == 0) printf(" ");
        }
        printf("\n");
    }
}

int main() {
    // Example 64-bit key
    uint64_t initialKey = 0x133457799BBCDFF1; // Example key (64 bits)

    // Array to hold the 16 subkeys
    uint64_t subkeys[16];

    // Generate the subkeys
    generateSubkeys(initialKey, subkeys);

    // Print the subkeys
    printSubkeys(subkeys);

    return 0;
}

