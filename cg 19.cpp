#include <stdio.h>
#include <stdint.h>
#include <string.h>
const int IP[] = { /* Initial Permutation Table */ };
const int FP[] = { /* Final Permutation Table */ };

const int PC1[] = { /* PC1 Table */ };
const int PC2[] = { /* PC2 Table */ };

const int E[] = { /* Expansion Table */ };

const int S[8][4][16] = { /* S-boxes */ };

const int P[] = { /* Permutation Table */ };

const int SHIFTS[] = { 1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1 };

typedef uint64_t uint48_t;


void key_schedule(uint64_t key, uint48_t subkeys[16]);
uint32_t feistel(uint32_t half_block, uint48_t subkey);
void des_encrypt(uint64_t plaintext, uint64_t key, uint64_t *ciphertext);
void triple_des_encrypt(uint64_t plaintext, uint64_t key1, uint64_t key2, uint64_t key3, uint64_t *ciphertext);
void cbc_encrypt(uint64_t *plaintext, uint64_t *ciphertext, int num_blocks, uint64_t key1, uint64_t key2, uint64_t key3, uint64_t iv);

int main() {
    uint64_t key1 = 0x133457799BBCDFF1; 
    uint64_t key2 = 0x1A1B1C1D1E1F1A1B; 
    uint64_t key3 = 0x2A2B2C2D2E2F2A2B; 
    uint64_t iv = 0x0123456789ABCDEF;   

    uint64_t plaintext[] = {0x0123456789ABCDEF, 0xFEDCBA9876543210}; 
    int num_blocks = sizeof(plaintext) / sizeof(uint64_t);
    uint64_t ciphertext[num_blocks];

    cbc_encrypt(plaintext, ciphertext, num_blocks, key1, key2, key3, iv);

    for (int i = 0; i < num_blocks; i++) {
        printf("Ciphertext block %d: %016lX\n", i, ciphertext[i]);
    }

    return 0;
}

void key_schedule(uint64_t key, uint48_t subkeys[16]) {
    uint64_t permuted_key = 0;
    for (int i = 0; i < 56; i++) {
        permuted_key |= ((key >> (64 - PC1[i])) & 1) << (55 - i);
    }

    uint32_t C = (permuted_key >> 28) & 0xFFFFFFF;
    uint32_t D = permuted_key & 0xFFFFFFF;

    for (int i = 0; i < 16; i++) {
        C = ((C << SHIFTS[i]) | (C >> (28 - SHIFTS[i]))) & 0xFFFFFFF;
        D = ((D << SHIFTS[i]) | (D >> (28 - SHIFTS[i]))) & 0xFFFFFFF;

        uint64_t combined_key = ((uint64_t)C << 28) | D;

        subkeys[i] = 0;
        for (int j = 0; j < 48; j++) {
            subkeys[i] |= ((combined_key >> (56 - PC2[j])) & 1) << (47 - j);
        }
    }
}

uint32_t feistel(uint32_t half_block, uint48_t subkey) {
    uint64_t expanded_half_block = 0;
    for (int i = 0; i < 48; i++) {
        expanded_half_block |= ((half_block >> (32 - E[i])) & 1) << (47 - i);
    }

    uint64_t xor_result = expanded_half_block ^ subkey;
    uint32_t sbox_output = 0;
    for (int i = 0; i < 8; i++) {
        int row = ((xor_result >> (47 - 6 * i)) & 0x20) | ((xor_result >> (42 - 6 * i)) & 0x01);
        int col = (xor_result >> (43 - 6 * i)) & 0x0F;
        sbox_output |= S[i][row][col] << (28 - 4 * i);
    }

    uint32_t permuted_sbox_output = 0;
    for (int i = 0; i < 32; i++) {
        permuted_sbox_output |= ((sbox_output >> (32 - P[i])) & 1) << (31 - i);
    }

    return permuted_sbox_output;
}

void des_encrypt(uint64_t plaintext, uint64_t key, uint64_t *ciphertext) {
    uint48_t subkeys[16];
    key_schedule(key, subkeys);

    uint64_t permuted_block = 0;
    for (int i = 0; i < 64; i++) {
        permuted_block |= ((plaintext >> (64 - IP[i])) & 1) << (63 - i);
    }

    uint32_t left = permuted_block >> 32;
    uint32_t right = permuted_block & 0xFFFFFFFF;

    for (int i = 0; i < 16; i++) {
        uint32_t new_right = left ^ feistel(right, subkeys[i]);
        left = right;
        right = new_right;
    }

    uint64_t combined_block = ((uint64_t)right << 32) | left;
    *ciphertext = 0;
    for (int i = 0; i < 64; i++) {
        *ciphertext |= ((combined_block >> (64 - FP[i])) & 1) << (63 - i);
    }
}

void triple_des_encrypt(uint64_t plaintext, uint64_t key1, uint64_t key2, uint64_t key3, uint64_t *ciphertext) {
    uint64_t temp1, temp2;
    des_encrypt(plaintext, key1, &temp1);  
    des_encrypt(temp1, key2, &temp2);      
    des_encrypt(temp2, key3, ciphertext);  
}

void cbc_encrypt(uint64_t *plaintext, uint64_t *ciphertext, int num_blocks, uint64_t key1, uint64_t key2, uint64_t key3, uint64_t iv) {
    uint64_t previous_block = iv;

    for (int i = 0; i < num_blocks; i++) {
        uint64_t xor_block = plaintext[i] ^ previous_block;
        triple_des_encrypt(xor_block, key1, key2, key3, &ciphertext[i]);
        previous_block = ciphertext[i];
    }
}

