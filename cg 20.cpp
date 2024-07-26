#include <stdio.h>
#include <string.h>
#include <stdint.h>

#define BLOCK_SIZE 8

void encrypt_block(const uint8_t input[BLOCK_SIZE], uint8_t output[BLOCK_SIZE], uint64_t key) {
    for (int i = 0; i < BLOCK_SIZE; i++) {
        output[i] = input[i] ^ (key & 0xFF); 
    }
}

void decrypt_block(const uint8_t input[BLOCK_SIZE], uint8_t output[BLOCK_SIZE], uint64_t key) {
    for (int i = 0; i < BLOCK_SIZE; i++) {
        output[i] = input[i] ^ (key & 0xFF); 
    }
}

void ecb_encrypt(const uint8_t *plaintext, uint8_t *ciphertext, size_t length, uint64_t key) {
    for (size_t i = 0; i < length; i += BLOCK_SIZE) {
        encrypt_block(plaintext + i, ciphertext + i, key);
    }
}

void ecb_decrypt(const uint8_t *ciphertext, uint8_t *plaintext, size_t length, uint64_t key) {
    for (size_t i = 0; i < length; i += BLOCK_SIZE) {
        decrypt_block(ciphertext + i, plaintext + i, key);
    }
}

int main() {
    uint64_t key = 0x1A2B3C4D;

    uint8_t plaintext[] = "Hello, World!"; 
    size_t length = sizeof(plaintext) - 1; 

    size_t padded_length = ((length + BLOCK_SIZE - 1) / BLOCK_SIZE) * BLOCK_SIZE;
    uint8_t padded_plaintext[padded_length];
    memcpy(padded_plaintext, plaintext, length);
    memset(padded_plaintext + length, 0, padded_length - length); 

    uint8_t ciphertext[padded_length];
    uint8_t decrypted[padded_length];

    ecb_encrypt(padded_plaintext, ciphertext, padded_length, key);
    printf("Ciphertext: ");
    for (size_t i = 0; i < padded_length; i++) {
        printf("%02X ", ciphertext[i]);
    }
    printf("\n");

    ecb_decrypt(ciphertext, decrypted, padded_length, key);
    printf("Decrypted: ");
    for (size_t i = 0; i < length; i++) {
        printf("%c", decrypted[i]);
    }
    printf("\n");

    return 0;
}

