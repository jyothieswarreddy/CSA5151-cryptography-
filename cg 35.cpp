#include <stdio.h>
#include <stdlib.h>

#define MAX_LENGTH 100

// Function to encrypt a single character using the one-time pad
char encrypt_char(char p, int shift) {
    if (p < 'A' || p > 'Z') {
        return p; // Non-alphabetic characters are returned unchanged
    }
    return (char)((((p - 'A') + shift) % 26) + 'A');
}

// Function to encrypt the message using the one-time pad
void encrypt_message(const char *plaintext, const int *key, int key_length) {
    printf("Ciphertext: ");
    for (int i = 0; plaintext[i] != '\0'; i++) {
        char c = encrypt_char(plaintext[i], key[i % key_length]);
        printf("%c", c);
    }
    printf("\n");
}

int main() {
    char plaintext[MAX_LENGTH];
    int key[MAX_LENGTH];
    int key_length;

    printf("Enter the plaintext (uppercase letters only): ");
    fgets(plaintext, sizeof(plaintext), stdin);

    printf("Enter the number of shifts in the key: ");
    scanf("%d", &key_length);

    printf("Enter the key (space-separated integers between 0 and 26): ");
    for (int i = 0; i < key_length; i++) {
        scanf("%d", &key[i]);
        if (key[i] < 0 || key[i] > 26) {
            printf("Error: Key values must be between 0 and 26.\n");
            return 1;
        }
    }

    encrypt_message(plaintext, key, key_length);

    return 0;
}
