#include <stdio.h>
#include <string.h>

#define BLOCK_SIZE 12

// Function to XOR two blocks of data
void xor_blocks(unsigned char *a, unsigned char *b, unsigned char *out, int len) {
    for (int i = 0; i < len; i++) {
        out[i] = a[i] ^ b[i];
    }
}

// ECB mode encryption/decryption
void ecb_mode(unsigned char *input, unsigned char *output, unsigned char *key, int len) {
    for (int i = 0; i < len; i += BLOCK_SIZE) {
        xor_blocks(input + i, key, output + i, BLOCK_SIZE);
    }
}

// CBC mode encryption
void cbc_mode_encrypt(unsigned char *input, unsigned char *output, unsigned char *key, unsigned char *iv, int len) {
    unsigned char block[BLOCK_SIZE];
    unsigned char prev_block[BLOCK_SIZE];
    memcpy(prev_block, iv, BLOCK_SIZE);

    for (int i = 0; i < len; i += BLOCK_SIZE) {
        xor_blocks(input + i, prev_block, block, BLOCK_SIZE);
        xor_blocks(block, key, output + i, BLOCK_SIZE);
        memcpy(prev_block, output + i, BLOCK_SIZE);
    }
}

// CBC mode decryption
void cbc_mode_decrypt(unsigned char *input, unsigned char *output, unsigned char *key, unsigned char *iv, int len) {
    unsigned char block[BLOCK_SIZE];
    unsigned char prev_block[BLOCK_SIZE];
    unsigned char temp[BLOCK_SIZE];
    memcpy(prev_block, iv, BLOCK_SIZE);

    for (int i = 0; i < len; i += BLOCK_SIZE) {
        xor_blocks(input + i, key, block, BLOCK_SIZE);
        xor_blocks(block, prev_block, output + i, BLOCK_SIZE);
        memcpy(prev_block, input + i, BLOCK_SIZE);
    }
}

// CFB mode encryption
void cfb_mode_encrypt(unsigned char *input, unsigned char *output, unsigned char *key, unsigned char *iv, int len) {
    unsigned char prev_block[BLOCK_SIZE];
    unsigned char block[BLOCK_SIZE];
    memcpy(prev_block, iv, BLOCK_SIZE);

    for (int i = 0; i < len; i += BLOCK_SIZE) {
        xor_blocks(prev_block, key, block, BLOCK_SIZE);
        xor_blocks(input + i, block, output + i, BLOCK_SIZE);
        memcpy(prev_block, output + i, BLOCK_SIZE);
    }
}

// CFB mode decryption
void cfb_mode_decrypt(unsigned char *input, unsigned char *output, unsigned char *key, unsigned char *iv, int len) {
    unsigned char prev_block[BLOCK_SIZE];
    unsigned char block[BLOCK_SIZE];
    memcpy(prev_block, iv, BLOCK_SIZE);

    for (int i = 0; i < len; i += BLOCK_SIZE) {
        xor_blocks(prev_block, key, block, BLOCK_SIZE);
        xor_blocks(input + i, block, output + i, BLOCK_SIZE);
        memcpy(prev_block, input + i, BLOCK_SIZE);
    }
}

int main() {
    unsigned char key[BLOCK_SIZE] ="mysecret";
    unsigned char iv[BLOCK_SIZE] = "initvect";
    unsigned char plaintext[16] = "HelloWorld1234";
    unsigned char ciphertext[16];
    unsigned char decryptedtext[16];

    int len = sizeof(plaintext);

    // ECB mode
    printf("ECB Mode:\n");
    ecb_mode(plaintext, ciphertext, key, len);
    ecb_mode(ciphertext, decryptedtext, key, len);
    printf("Decrypted text: %s\n\n", decryptedtext);

    // CBC mode
    printf("CBC Mode:\n");
    cbc_mode_encrypt(plaintext, ciphertext, key, iv, len);
    cbc_mode_decrypt(ciphertext, decryptedtext, key, iv, len);
    printf("Decrypted text: %s\n\n", decryptedtext);

    // CFB mode
    printf("CFB Mode:\n");
    cfb_mode_encrypt(plaintext, ciphertext, key, iv, len);
    cfb_mode_decrypt(ciphertext, decryptedtext, key, iv, len);
    printf("Decrypted text: %s\n\n", decryptedtext);

    return 0;
}

