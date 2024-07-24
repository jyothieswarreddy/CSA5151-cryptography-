#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LENGTH 100

// Function to encrypt the plaintext using the one-time pad Vigenère cipher
void encrypt(char *plaintext, int *key, int key_length, char *ciphertext) {
    int len = strlen(plaintext);
    for (int i = 0; i < len; i++) {
        if (isalpha(plaintext[i])) {
            int shift = key[i % key_length]; // Get the corresponding key value
            char base = islower(plaintext[i]) ? 'a' : 'A'; // Determine base for case
            ciphertext[i] = (plaintext[i] - base + shift) % 26 + base; // Encrypt
        } else {
            ciphertext[i] = plaintext[i]; // Keep non-alpha characters unchanged
        }
    }
    ciphertext[len] = '\0'; // Null terminate the string
}

// Function to decrypt the ciphertext using the one-time pad Vigenère cipher
void decrypt(char *ciphertext, int *key, int key_length, char *plaintext) {
    int len = strlen(ciphertext);
    for (int i = 0; i < len; i++) {
        if (isalpha(ciphertext[i])) {
            int shift = key[i % key_length]; // Get the corresponding key value
            char base = islower(ciphertext[i]) ? 'a' : 'A'; // Determine base for case
            plaintext[i] = (ciphertext[i] - base - shift + 26) % 26 + base; // Decrypt
        } else {
            plaintext[i] = ciphertext[i]; // Keep non-alpha characters unchanged
        }
    }
    plaintext[len] = '\0'; // Null terminate the string
}

// Function to generate a key from ciphertext and desired plaintext
void generateKey(char *ciphertext, char *desired_plaintext, int *key) {
    int len = strlen(ciphertext);
    for (int i = 0; i < len; i++) {
        if (isalpha(ciphertext[i]) && isalpha(desired_plaintext[i])) {
            int shift = (ciphertext[i] - desired_plaintext[i] + 26) % 26; // Calculate key
            key[i] = shift;
        } else {
            key[i] = 0; // Non-alpha characters do not affect the key
        }
    }
}

int main() {
    char plaintext[] = "send more money";
    char ciphertext[MAX_LENGTH];
    char decrypted[MAX_LENGTH];
    
    // Key stream for encryption
    int key_stream[] = {9, 0, 1, 7, 23, 15, 21, 14, 11, 11, 2, 8, 9};
    int key_length = sizeof(key_stream) / sizeof(key_stream[0]);

    // Encrypt the plaintext
    encrypt(plaintext, key_stream, key_length, ciphertext);
    printf("Ciphertext: %s\n", ciphertext);
    
    // Desired plaintext for decryption
    char desired_plaintext[] = "cash not needed";
    int generated_key[MAX_LENGTH];
    
    // Generate key from ciphertext and desired plaintext
    generateKey(ciphertext, desired_plaintext, generated_key);
    
    // Decrypt the ciphertext using the generated key
    decrypt(ciphertext, generated_key, key_length, decrypted);
    
    printf("Generated Key: ");
    for (int i = 0; i < strlen(ciphertext); i++) {
        if (isalpha(ciphertext[i])) {
            printf("%d ", generated_key[i]);
        }
    }
    printf("\nDecrypted with Generated Key: %s\n", decrypted);

    return 0;
}
