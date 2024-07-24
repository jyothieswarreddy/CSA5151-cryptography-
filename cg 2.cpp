#include <stdio.h>
#include <string.h>
#include <ctype.h>

// Function to encrypt the text using monoalphabetic substitution cipher
void monoalphabeticCipher(char *text, const char *key) {
    char plaintextAlphabet[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    char ciphertextAlphabet[26];
    
    // Create ciphertext alphabet from key
    strncpy(ciphertextAlphabet, key, 26);

    for (int i = 0; text[i] != '\0'; i++) {
        char ch = text[i];
        if (isalpha(ch)) {
            int isUpper = isupper(ch);
            ch = toupper(ch);
            // Find the position of the character in the plaintext alphabet
            char *pos = strchr(plaintextAlphabet, ch);
            if (pos) {
                int index = pos - plaintextAlphabet;
                // Replace with the corresponding character in the ciphertext alphabet
                text[i] = isUpper ? ciphertextAlphabet[index] : tolower(ciphertextAlphabet[index]);
            }
        }
    }
}

int main() {
    char text[100];
    char key[27];

    // Input the text to be encrypted
    printf("Enter the text to be encrypted: ");
    fgets(text, sizeof(text), stdin);
    text[strcspn(text, "\n")] = '\0'; // Remove the newline character

    // Input the substitution key (26 unique uppercase letters)
    printf("Enter the substitution key (26 unique uppercase letters): ");
    fgets(key, sizeof(key), stdin);
    key[strcspn(key, "\n")] = '\0'; // Remove the newline character

    // Validate the key length and uniqueness
    if (strlen(key) != 26) {
        printf("Key must be exactly 26 characters long.\n");
        return 1;
    }
    
    int charSet[26] = {0};
    for (int i = 0; i < 26; i++) {
        if (!isupper(key[i]) || charSet[key[i] - 'A']) {
            printf("Key must contain 26 unique uppercase letters.\n");
            return 1;
        }
        charSet[key[i] - 'A'] = 1;
    }

    // Encrypt the text
    monoalphabeticCipher(text, key);

    // Output the encrypted text
    printf("Encrypted text: %s\n", text);

    return 0;
}

