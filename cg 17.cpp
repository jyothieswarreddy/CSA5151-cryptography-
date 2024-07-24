#include <stdio.h>
#include <stdint.h>
#include <string.h>

#define DES_BLOCK_SIZE 8 // DES operates on 64-bit blocks
#define DES_KEY_SIZE 8   // DES uses a 64-bit key (56 bits effective)

// Initial Permutation Table
static const int IP[64] = {
    58, 50, 42, 34, 26, 18, 10, 2,
    60, 52, 44, 36, 28, 20, 12, 4,
    62, 54, 46, 38, 30, 22, 14, 6,
    64, 56, 48, 40, 32, 24, 16, 8,
    57, 49, 41, 33, 25, 17, 9, 1,
    59, 51, 43, 35, 27, 19, 11, 3,
    61, 53, 45, 37, 29, 21, 13, 5,
    63, 55, 47, 39, 31, 23, 15, 7
};

// Inverse Initial Permutation Table
static const int IP_INV[64] = {
    40, 8, 48, 16, 56, 24, 32, 0,
    41, 9, 49, 17, 57, 25, 33, 1,
    42, 10, 50, 18, 58, 26, 34, 2,
    43, 11, 51, 19, 59, 27, 35, 3,
    44, 12, 52, 20, 60, 28, 36, 4,
    45, 13, 53, 21, 61, 29, 37, 5,
    46, 14, 54, 22, 62, 30, 38, 6,
    47, 15, 55, 23, 63, 31, 39, 7
};

// Key Schedule Shifts
static const int shifts[16] = {1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1};

// Permutation Table for Key Compression
static const int PC1[56] = {
    57, 49, 41, 33, 25, 17, 9,
    1, 58, 50, 42, 34, 26, 18,
    10, 2, 59, 51, 43, 35, 27,
    19, 11, 3, 60, 52, 44, 36,
    63, 55, 47, 39, 31, 23, 15,
    7, 62, 54, 46, 38, 30, 22,
    14, 6, 61, 53, 45, 37, 29,
    21, 13, 5, 28, 20, 12, 4
};

// Permutation Table for Key Compression
static const int PC2[48] = {
    14, 17, 11, 24, 1, 5,
    3, 28, 15, 6, 21, 10,
    23, 19, 12, 4, 26, 8,
    16, 7, 27, 20, 13, 2,
    41, 52, 31, 37, 47, 55,
    30, 40, 51, 45, 33, 48,
    44, 49, 39, 56, 34, 53,
    46, 42, 50, 36, 29, 32
};

// S-boxes for the Feistel function
static const int S[8][4][16] = {
    {
        {14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7},
        {0, 15, 7, 4, 14, 2, 13, 1, 10, 6, 12, 11, 9, 5, 3, 8},
        {4, 1, 14, 8, 13, 6, 2, 11, 15, 12, 9, 7, 3, 10, 5, 0},
        {15, 2, 8, 14, 3, 4, 9, 7, 5, 6, 10, 11, 12, 0, 1, 13}
    },
    {
        {15, 1, 8, 14, 6, 11, 3, 4, 9, 7, 2, 13, 12, 0, 5, 10},
        {3, 13, 4, 7, 15, 2, 8, 14, 12, 0, 1, 10, 6, 9, 11, 5},
        {0, 14, 7, 11, 10, 4, 13, 1, 5, 8, 12, 6, 9, 3, 2, 15},
        {13, 8, 10, 1, 3, 15, 4, 2, 11, 6, 7, 12, 0, 5, 14, 9}
    },
    {
        {10, 0, 9, 14, 6, 3, 15, 5, 1, 13, 2, 8, 12, 7, 4, 11},
        {13, 7, 0, 9, 3, 4, 6, 10, 2, 8, 5, 14, 12, 11, 15, 1},
        {13, 6, 4, 9, 8, 15, 3, 0, 1, 2, 7, 10, 14, 12, 11, 5},
        {2, 8, 12, 1, 10, 3, 15, 4, 9, 5, 6, 11, 0, 14, 7, 13}
    },
    {
        {7, 13, 14, 3, 0, 6, 9, 10, 1, 2, 8, 5, 11, 12, 4, 15},
        {13, 8, 11, 5, 6, 15, 0, 3, 4, 7, 2, 12, 1, 10, 14, 9},
        {10, 6, 9, 0, 12, 11, 7, 13, 15, 1, 3, 14, 5, 2, 8, 4},
        {3, 15, 0, 6, 10, 1, 13, 8, 9, 4, 5, 11, 12, 7, 2, 14}
    },
    {
        {2, 12, 4, 1, 7, 10, 11, 6, 9, 5, 3, 14, 0, 15, 13, 8},
        {4, 2, 1, 11, 10, 13, 7, 8, 15, 9, 12, 5, 6, 3, 0, 14},
        {11, 1, 10, 4, 2, 8, 5, 3, 15, 13, 12, 9, 7, 6, 0, 14},
        {12, 10, 15, 4, 1, 5, 3, 9, 14, 7, 6, 11, 8, 0, 13, 2}
    },
    {
        {12, 1, 10, 15, 9, 2, 6, 8, 0, 13, 3, 4, 14, 7, 5, 11},
        {10, 15, 4, 2, 7, 12, 9, 5, 6, 1, 13, 0, 3, 14, 11, 8},
        {9, 14, 15, 5, 2, 8, 12, 3, 7, 0, 4, 10, 1, 13, 11, 6},
        {4, 3, 2, 12, 9, 5, 15, 10, 14, 1, 11, 8, 0, 6, 13, 7}
    },
    {
        {4, 11, 2, 14, 15, 0, 8, 13, 3, 7, 12, 5, 6, 10, 9, 1},
        {13, 0, 11, 7, 4, 9, 1, 10, 14, 3, 5, 12, 2, 15, 8, 6},
        {1, 4, 11, 13, 12, 3, 7, 14, 10, 15, 6, 8, 0, 5, 9, 2},
        {6, 1, 13, 8, 4, 10, 7, 11, 0, 14, 9, 3, 15, 5, 2, 12}
    }
};

// Function to perform the initial permutation
void initial_permutation(uint64_t *block) {
    uint64_t temp = 0;
    for (int i = 0; i < 64; i++) {
        if ((*block >> (64 - IP[i])) & 1) {
            temp |= (1ULL << (63 - i));
        }
    }
    *block = temp;
}

// Function to perform the inverse initial permutation
void inverse_initial_permutation(uint64_t *block) {
    uint64_t temp = 0;
    for (int i = 0; i < 64; i++) {
        if ((*block >> (64 - IP_INV[i])) & 1) {
            temp |= (1ULL << (63 - i));
        }
    }
    *block = temp;
}

// Function to generate the round keys
void generate_keys(uint64_t key, uint64_t keys[16]) {
    uint64_t C = 0, D = 0;
    uint64_t K = 0;

    // Apply PC1 to the key
    for (int i = 0; i < 56; i++) {
        if ((key >> (64 - PC1[i])) & 1) {
            if (i < 28) {
                C |= (1ULL << (27 - i % 28));
            } else {
                D |= (1ULL << (27 - (i - 28) % 28));
            }
        }
    }

    // Generate 16 keys
    for (int i = 0; i < 16; i++) {
        // Shift C and D
        C = ((C << shifts[i]) | (C >> (28 - shifts[i]))) & 0x0FFFFFFF;
        D = ((D << shifts[i]) | (D >> (28 - shifts[i]))) & 0x0FFFFFFF;

        // Combine C and D
        K = (C << 28) | D;

        // Apply PC2 to get the round key
        keys[i] = 0;
        for (int j = 0; j < 48; j++) {
            if ((K >> (56 - PC2[j])) & 1) {
                keys[i] |= (1ULL << (47 - j));
            }
        }
    }
}

// Function to perform the Feistel function
uint64_t feistel_function(uint32_t R, uint64_t key) {
    uint64_t expanded_R = 0;

    // Expand R from 32 bits to 48 bits
    for (int i = 0; i < 48; i++) {
        if ((R >> (32 - (i + 1))) & 1) {
            expanded_R |= (1ULL << (47 - i));
        }
    }

    // XOR with the key
    expanded_R ^= key;

    // S-box substitution
    uint32_t S_output = 0;
    for (int i = 0; i < 8; i++) {
        int row = ((expanded_R >> (42 - (6 * i))) & 0x20) | ((expanded_R >> (41 - (6 * i))) & 1);
        int col = (expanded_R >> (43 - (6 * i))) & 0x0F;
        S_output |= (S[i][row][col] << (28 - (4 * i)));
    }

    // Permutation P
    uint32_t P_output = 0;
    static const int P[32] = {
        16, 7, 20, 21, 29, 12, 28, 17,
        1, 15, 23, 26, 5, 18, 31, 10,
        2, 8, 24, 14, 32, 27, 3, 9,
        19, 13, 30, 6, 22, 11, 4, 25
    };
    for (int i = 0; i < 32; i++) {
        if ((S_output >> (32 - P[i])) & 1) {
            P_output |= (1ULL << (31 - i));
        }
    }

    return P_output;
}

// Function to perform DES decryption
void des_decrypt(uint64_t ciphertext, uint64_t keys[16], uint64_t *plaintext) {
    uint64_t block = ciphertext;

    // Initial permutation
    initial_permutation(&block);

    // Split into left and right halves
    uint32_t L = (block >> 32) & 0xFFFFFFFF;
    uint32_t R = block & 0xFFFFFFFF;

    // 16 rounds of processing
    for (int i = 15; i >= 0; i--) {
        uint32_t temp_R = R;
        R = L ^ feistel_function(R, keys[i]);
        L = temp_R;
    }

    // Combine L and R
    block = ((uint64_t)L << 32) | R;

    // Inverse initial permutation
    inverse_initial_permutation(&block);

    *plaintext = block;
}

int main() {
    uint64_t key, ciphertext, plaintext;
    uint64_t keys[16];

    // Input key and ciphertext
    printf("Enter the 64-bit key (in hex): ");
    scanf("%lx", &key);
    printf("Enter the 64-bit ciphertext (in hex): ");
    scanf("%lx", &ciphertext);

    // Generate keys
    generate_keys(key, keys);

    // Perform DES decryption
    des_decrypt(ciphertext, keys, &plaintext);

    // Output plaintext
    printf("Decrypted plaintext: %016lx\n", plaintext);
    return 0;
}
