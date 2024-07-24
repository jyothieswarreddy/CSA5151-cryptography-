#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX_MESSAGE_LENGTH 1000
#define MAX_KEYWORD_LENGTH 100

// Function to generate the cipher sequence from the keyword
void generate_cipher_sequence(char* keyword, char* cipher_sequence) {
    int i, j, index = 0;
    int used[26] = {0};

    // Add the keyword characters to the cipher sequence
    for (i = 0; i < strlen(keyword); i++) {
        if (isalpha(keyword[i])) {
            char c = tolower(keyword[i]);
            if (!used[c - 'a']) {
                cipher_sequence[index++] = c;
                used[c - 'a'] = 1;
            }
        }
    }

    // Add the unused letters to the cipher sequence
    for (i = 0; i < 26; i++) {
        if (!used[i]) {
            cipher_sequence[index++] = 'a' + i;
        }
    }
    cipher_sequence[index] = '\0';
}

// Function to encrypt the plaintext using the cipher sequence
void encrypt(char* plaintext, char* cipher_sequence, char* ciphertext) {
    int i;
    for (i = 0; i < strlen(plaintext); i++) {
        if (isalpha(plaintext[i])) {
            char base = isupper(plaintext[i]) ? 'A' : 'a';
            int index = plaintext[i] - base;
            ciphertext[i] = toupper(cipher_sequence[index]);
        } else {
            ciphertext[i] = plaintext[i];
        }
    }
    ciphertext[i] = '\0';
}

// Function to decrypt the ciphertext using the cipher sequence
void decrypt(char* ciphertext, char* cipher_sequence, char* plaintext) {
    int i;
    for (i = 0; i < strlen(ciphertext); i++) {
        if (isalpha(ciphertext[i])) {
            char base = isupper(ciphertext[i]) ? 'A' : 'a';
            int index = toupper(ciphertext[i]) - 'A';
            plaintext[i] = base + strchr(cipher_sequence, tolower(ciphertext[i])) - cipher_sequence;
        } else {
            plaintext[i] = ciphertext[i];
        }
    }
    plaintext[i] = '\0';
}

int main() {
    char plaintext[MAX_MESSAGE_LENGTH];
    char keyword[MAX_KEYWORD_LENGTH];
    char cipher_sequence[26 + 1];
    char ciphertext[MAX_MESSAGE_LENGTH];
    char decrypted_text[MAX_MESSAGE_LENGTH];

    printf("Enter the plaintext: ");
    fgets(plaintext, MAX_MESSAGE_LENGTH, stdin);
    plaintext[strcspn(plaintext, "\n")] = '\0'; // Remove the newline character

    printf("Enter the keyword: ");
    fgets(keyword, MAX_KEYWORD_LENGTH, stdin);
    keyword[strcspn(keyword, "\n")] = '\0'; // Remove the newline character

    generate_cipher_sequence(keyword, cipher_sequence);
    printf("Cipher sequence: %s\n", cipher_sequence);

    encrypt(plaintext, cipher_sequence, ciphertext);
    printf("Ciphertext: %s\n", ciphertext);

    decrypt(ciphertext, cipher_sequence, decrypted_text);
    printf("Decrypted text: %s\n", decrypted_text);

    return 0;
}
