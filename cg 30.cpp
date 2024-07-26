#include <stdio.h>
#include <stdint.h>
#include <string.h>

// Define the block size and key size
#define BLOCK_SIZE 16  // Block size in bytes (128 bits for simplicity)
#define KEY_SIZE 16    // Key size in bytes

// Simple XOR encryption for demonstration (not secure in real applications)
void xor_encrypt(uint8_t *block, uint8_t *key, uint8_t *output) {
    for (int i = 0; i < BLOCK_SIZE; i++) {
        output[i] = block[i] ^ key[i];
    }
}

// Compute CBC-MAC for a single block message
void cbc_mac(uint8_t *key, uint8_t *message, uint8_t *mac) {
    uint8_t iv[BLOCK_SIZE] = {0};  // Initialization vector (IV) set to zero
    uint8_t encrypted[BLOCK_SIZE];

    // XOR the message with the IV
    for (int i = 0; i < BLOCK_SIZE; i++) {
        encrypted[i] = message[i] ^ iv[i];
    }

    // Encrypt the block with the key
    xor_encrypt(encrypted, key, mac);
}

// Compute CBC-MAC for a two-block message
void cbc_mac_two_blocks(uint8_t *key, uint8_t *message1, uint8_t *message2, uint8_t *mac) {
    uint8_t iv[BLOCK_SIZE] = {0};  // Initialization vector (IV) set to zero
    uint8_t encrypted[BLOCK_SIZE];

    // First block
    for (int i = 0; i < BLOCK_SIZE; i++) {
        encrypted[i] = message1[i] ^ iv[i];
    }

    xor_encrypt(encrypted, key, encrypted);

    // Second block
    for (int i = 0; i < BLOCK_SIZE; i++) {
        encrypted[i] = message2[i] ^ encrypted[i];
    }

    xor_encrypt(encrypted, key, mac);
}

void print_hex(uint8_t *data, size_t size) {
    for (size_t i = 0; i < size; i++) {
        printf("%02x ", data[i]);
    }
    printf("\n");
}

int main() {
    uint8_t key[KEY_SIZE] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,
                             0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10};

    uint8_t message[BLOCK_SIZE] = {0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88,
                                   0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x00};

    uint8_t mac[BLOCK_SIZE];
    uint8_t two_block_mac[BLOCK_SIZE];
    uint8_t x_xor_t[BLOCK_SIZE];

    // Compute CBC-MAC for the one-block message X
    cbc_mac(key, message, mac);

    printf("MAC(K, X): ");
    print_hex(mac, BLOCK_SIZE);

    // Compute X ? T
    for (int i = 0; i < BLOCK_SIZE; i++) {
        x_xor_t[i] = message[i] ^ mac[i];
    }

    printf("X ? T: ");
    print_hex(x_xor_t, BLOCK_SIZE);

    // Compute CBC-MAC for the two-block message X || (X ? T)
    cbc_mac_two_blocks(key, message, x_xor_t, two_block_mac);

    printf("MAC(K, X || (X ? T)): ");
    print_hex(two_block_mac, BLOCK_SIZE);

    return 0;
}

