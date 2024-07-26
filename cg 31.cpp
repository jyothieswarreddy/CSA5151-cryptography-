#include <stdio.h>
#include <stdint.h>

// Constants for block sizes
#define BLOCK_SIZE_64 0x1b
#define BLOCK_SIZE_128 0x87

// Function to generate subkeys
void generate_subkeys(uint64_t *k1, uint64_t *k2, uint64_t block_cipher_output, int block_size) {
    // Shift left by 1 bit
    *k1 = (block_cipher_output << 1);

    // Conditionally XOR constant based on block size
    if (block_size == 64) {
        if (block_cipher_output & 0x8000000000000000) {
            *k1 ^= BLOCK_SIZE_64;
        }
    } else {  // Assume 128-bit block size
        if (block_cipher_output & 0x8000000000000000) {
            *k1 ^= BLOCK_SIZE_128;
        }
    }

    // Generate k2 from k1
    *k2 = (*k1 << 1);
    if (*k1 & 0x8000000000000000) {
        *k2 ^= (block_size == 64) ? BLOCK_SIZE_64 : BLOCK_SIZE_128;
    }
}

int main() {
    uint64_t block_cipher_output = 0x0000000000000000;  // Output of block cipher applied to 0 bits
    uint64_t k1, k2;

    // Generate subkeys for 64-bit block size
    generate_subkeys(&k1, &k2, block_cipher_output, 64);
    printf("Subkeys for 64-bit block size:\n");
    printf("k1: 0x%016llx\n", k1);
    printf("k2: 0x%016llx\n", k2);

    // Generate subkeys for 128-bit block size
    generate_subkeys(&k1, &k2, block_cipher_output, 128);
    printf("\nSubkeys for 128-bit block size:\n");
    printf("k1: 0x%016llx\n", k1);
    printf("k2: 0x%016llx\n", k2);

    return 0;
}
