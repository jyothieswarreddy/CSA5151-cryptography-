#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX_KEY_LENGTH 100
#define MAX_MESSAGE_LENGTH 1000

// Function to encrypt the plaintext using the polyalphabetic substitution cipher
void encrypt(char* plaintext, char* key, char* ciphertext) {
    int plaintext_len = strlen(plaintext);
    int key_len = strlen(key);
    int i;

    for (i = 0; i < plaintext_len; i++) {
        if (isalpha(plaintext[i])) {
            char base = isupper(plaintext[i]) ? 'A' : 'a';
            int plaintext_index = plaintext[i] - base;
            int key_index = key[i % key_len] - base;
            int ciphertext_index = (plaintext_index + key_index) % 26;
            ciphertext[i] = base + ciphertext_index;
        } else {
            ciphertext[i] = plaintext[i];
        }
    }
    ciphertext[i] = '\0';
}

int main() {
    char plaintext[MAX_MESSAGE_LENGTH];
    char key[MAX_KEY_LENGTH];
    char ciphertext[MAX_MESSAGE_LENGTH];

    printf("Enter the plaintext: ");
    fgets(plaintext, MAX_MESSAGE_LENGTH, stdin);
    plaintext[strcspn(plaintext, "\n")] = '\0'; // Remove the newline character

    printf("Enter the key: ");
    fgets(key, MAX_KEY_LENGTH, stdin);
    key[strcspn(key, "\n")] = '\0'; // Remove the newline character

    encrypt(plaintext, key, ciphertext);

    printf("Ciphertext: %s\n", ciphertext);

    return 0;
}
