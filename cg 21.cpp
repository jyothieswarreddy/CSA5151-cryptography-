#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define AES_BLOCK_SIZE 16

void handle_errors() {
    fprintf(stderr, "An error occurred.\n");
    exit(EXIT_FAILURE);
}

void aes_encrypt(const unsigned char *input, unsigned char *output, const unsigned char *key) {
    memcpy(output, input, AES_BLOCK_SIZE);
}

void aes_decrypt(const unsigned char *input, unsigned char *output, const unsigned char *key) {

    memcpy(output, input, AES_BLOCK_SIZE);
}

void pad_plaintext(unsigned char *plaintext, size_t *length) {
    size_t padding_length = AES_BLOCK_SIZE - (*length % AES_BLOCK_SIZE);
    for (size_t i = *length; i < *length + padding_length; i++) {
        plaintext[i] = (unsigned char)padding_length;
    }
    *length += padding_length;
}

void unpad_plaintext(unsigned char *plaintext, size_t *length) {
    size_t padding_length = plaintext[*length - 1];
    *length -= padding_length;
}

void aes_encrypt_ecb(const unsigned char *plaintext, size_t plaintext_len,
                     const unsigned char *key, unsigned char *ciphertext) {
    for (size_t i = 0; i < plaintext_len; i += AES_BLOCK_SIZE) {
        aes_encrypt(plaintext + i, ciphertext + i, key);
    }
}

void aes_decrypt_ecb(const unsigned char *ciphertext, size_t ciphertext_len,
                     const unsigned char *key, unsigned char *plaintext) {
    for (size_t i = 0; i < ciphertext_len; i += AES_BLOCK_SIZE) {
        aes_decrypt(ciphertext + i, plaintext + i, key);
    }
}

void aes_encrypt_cbc(const unsigned char *plaintext, size_t plaintext_len,
                     const unsigned char *key, unsigned char *iv, unsigned char *ciphertext) {
    unsigned char previous_block[AES_BLOCK_SIZE];
    memcpy(previous_block, iv, AES_BLOCK_SIZE);

    for (size_t i = 0; i < plaintext_len; i += AES_BLOCK_SIZE) {
        unsigned char block[AES_BLOCK_SIZE];
        for (size_t j = 0; j < AES_BLOCK_SIZE; j++) {
            block[j] = plaintext[i + j] ^ previous_block[j];
        }
        aes_encrypt(block, ciphertext + i, key);
        memcpy(previous_block, ciphertext + i, AES_BLOCK_SIZE);
    }
}

void aes_decrypt_cbc(const unsigned char *ciphertext, size_t ciphertext_len,
                     const unsigned char *key, unsigned char *iv, unsigned char *plaintext) {
    unsigned char previous_block[AES_BLOCK_SIZE];
    memcpy(previous_block, iv, AES_BLOCK_SIZE);

    for (size_t i = 0; i < ciphertext_len; i += AES_BLOCK_SIZE) {
        unsigned char block[AES_BLOCK_SIZE];
        aes_decrypt(ciphertext + i, block, key);
        for (size_t j = 0; j < AES_BLOCK_SIZE; j++) {
            plaintext[i + j] = block[j] ^ previous_block[j];
        }
        memcpy(previous_block, ciphertext + i, AES_BLOCK_SIZE);
    }
}

void aes_encrypt_cfb(const unsigned char *plaintext, size_t plaintext_len,
                     const unsigned char *key, unsigned char *iv, unsigned char *ciphertext) {
    unsigned char feedback[AES_BLOCK_SIZE];
    memcpy(feedback, iv, AES_BLOCK_SIZE);

    for (size_t i = 0; i < plaintext_len; i += AES_BLOCK_SIZE) {
        unsigned char block[AES_BLOCK_SIZE];
        unsigned char encrypted_feedback[AES_BLOCK_SIZE];
        aes_encrypt(feedback, encrypted_feedback, key);
        for (size_t j = 0; j < AES_BLOCK_SIZE; j++) {
            block[j] = plaintext[i + j] ^ encrypted_feedback[j];
        }
        memcpy(ciphertext + i, block, AES_BLOCK_SIZE);
        memcpy(feedback, ciphertext + i, AES_BLOCK_SIZE);
    }
}

void aes_decrypt_cfb(const unsigned char *ciphertext, size_t ciphertext_len,
                     const unsigned char *key, unsigned char *iv, unsigned char *plaintext) {
    unsigned char feedback[AES_BLOCK_SIZE];
    memcpy(feedback, iv, AES_BLOCK_SIZE);

    for (size_t i = 0; i < ciphertext_len; i += AES_BLOCK_SIZE) {
        unsigned char block[AES_BLOCK_SIZE];
        unsigned char encrypted_feedback[AES_BLOCK_SIZE];
        aes_encrypt(feedback, encrypted_feedback, key);
        for (size_t j = 0; j < AES_BLOCK_SIZE; j++) {
            block[j] = ciphertext[i + j] ^ encrypted_feedback[j];
        }
        memcpy(plaintext + i, block, AES_BLOCK_SIZE);
        memcpy(feedback, ciphertext + i, AES_BLOCK_SIZE);
    }
}

int main() {
    const unsigned char plaintext[] = "This is a test message for AES modes.";
    size_t plaintext_len = strlen((const char *)plaintext);
    
    unsigned char key[16] = {0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF,
                             0xFE, 0xDC, 0xBA, 0x98, 0x76, 0x54, 0x32, 0x10};
    unsigned char iv[AES_BLOCK_SIZE] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
                                        0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F};

    pad_plaintext((unsigned char *)plaintext, &plaintext_len);

    size_t ciphertext_len = plaintext_len;
    unsigned char *ciphertext_ecb = (unsigned char *)malloc(ciphertext_len);
    unsigned char *ciphertext_cbc = (unsigned char *)malloc(ciphertext_len);
    unsigned char *ciphertext_cfb = (unsigned char *)malloc(ciphertext_len);
    unsigned char *decrypted_plaintext = (unsigned char *)malloc(ciphertext_len);
    if (!ciphertext_ecb || !ciphertext_cbc || !ciphertext_cfb || !decrypted_plaintext) {
        handle_errors();
    }

    aes_encrypt_ecb((unsigned char *)plaintext, plaintext_len, key, ciphertext_ecb);
    aes_decrypt_ecb(ciphertext_ecb, ciphertext_len, key, decrypted_plaintext);
    unpad_plaintext(decrypted_plaintext, &plaintext_len);
    printf("ECB Mode - Decrypted plaintext: %s\n", decrypted_plaintext);

    aes_encrypt_cbc((unsigned char *)plaintext, plaintext_len, key, iv, ciphertext_cbc);
    aes_decrypt_cbc(ciphertext_cbc, ciphertext_len, key, iv, decrypted_plaintext);
    unpad_plaintext(decrypted_plaintext, &plaintext_len);
    printf("CBC Mode - Decrypted plaintext: %s\n", decrypted_plaintext);

    aes_encrypt_cfb((unsigned char *)plaintext, plaintext_len, key, iv, ciphertext_cfb);
    aes_decrypt_cfb(ciphertext_cfb, ciphertext_len, key, iv, decrypted_plaintext);
    unpad_plaintext(decrypted_plaintext, &plaintext_len);
    printf("CFB Mode - Decrypted plaintext: %s\n", decrypted_plaintext);

    free(ciphertext_ecb);
    free(ciphertext_cbc);
    free(ciphertext_cfb);
    free(decrypted_plaintext);
    
    return 0;
}
