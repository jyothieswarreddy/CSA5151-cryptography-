#include <stdio.h>
#include <string.h>
#include <ctype.h>
#define MAX_MESSAGE_LENGTH 1000
void encrypt(char* plaintext, int a, int b, char* ciphertext) {
    int plaintext_len = strlen(plaintext);
    int i;
    for (i = 0; i < plaintext_len; i++) {
        if (isalpha(plaintext[i])) {
            char base = isupper(plaintext[i]) ? 'A' : 'a';
            int plaintext_index = plaintext[i] - base;
            int ciphertext_index = (a * plaintext_index + b) % 26;
            ciphertext[i] = base + ciphertext_index;
        } else {
            ciphertext[i] = plaintext[i];
        }
    }
    ciphertext[i] = '\0';
}
int main() {
    char plaintext[MAX_MESSAGE_LENGTH];
    int a, b;
    char ciphertext[MAX_MESSAGE_LENGTH];
    printf("Enter the plaintext: ");
    fgets(plaintext, MAX_MESSAGE_LENGTH, stdin);
    plaintext[strcspn(plaintext, "\n")] = '\0'; 
    printf("Enter the value of a: ");
    scanf("%d", &a);
    printf("Enter the value of b: ");
    scanf("%d", &b);
    encrypt(plaintext, a, b, ciphertext);
    printf("Ciphertext: %s\n", ciphertext);
    return 0;
}
