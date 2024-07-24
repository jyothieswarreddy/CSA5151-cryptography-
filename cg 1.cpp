#include <stdio.h>
#include <ctype.h>

// Function to encrypt the text using Caesar Cipher
void caesarCipher(char *text, int k) {
    for (int i = 0; text[i] != '\0'; i++) {
        char ch = text[i];
        if (isalpha(ch)) {
            char offset = isupper(ch) ? 'A' : 'a';
            text[i] = ((ch - offset + k) % 26) + offset;
        }
    }
}

int main() {
    char text[100];
    int k;

    // Input the string to be encrypted
    printf("Enter the text to be encrypted: ");
    fgets(text, sizeof(text), stdin);

    // Input the shift value k
    printf("Enter the shift value (k): ");
    scanf("%d", &k);

    // Ensure k is within the valid range [1, 25]
    if (k < 1 || k > 25) {
        printf("Shift value must be in the range 1 through 25.\n");
        return 1;
    }

    // Encrypt the text
    caesarCipher(text, k);

    // Output the encrypted text
    printf("Encrypted text: %s\n", text);

    return 0;
}

