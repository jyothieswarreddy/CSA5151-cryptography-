#include <stdio.h>
#include <stdlib.h>

int gcd(int a, int b) {
    while (b != 0) {
        int temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

int mod_inverse(int a, int m) {
    for (int x = 1; x < m; x++) {
        if ((a * x) % m == 1) {
            return x;
        }
    }
    return -1; // No modular inverse exists
}

char encrypt_char(char p, int a, int b) {
    if (p < 'A' || p > 'Z') {
        return p; // Non-alphabetic characters are returned unchanged
    }
    return (char)((a * (p - 'A') + b) % 26 + 'A');
}

void encrypt_message(const char *plaintext, int a, int b) {
    printf("Ciphertext: ");
    for (int i = 0; plaintext[i] != '\0'; i++) {
        char c = encrypt_char(plaintext[i], a, b);
        printf("%c", c);
    }
    printf("\n");
}

int main() {
    char plaintext[100];
    int a, b;

    printf("Enter the plaintext (uppercase letters only): ");
    fgets(plaintext, sizeof(plaintext), stdin);

    printf("Enter value for a (must be coprime to 26): ");
    scanf("%d", &a);
    printf("Enter value for b: ");
    scanf("%d", &b);

    // Check if 'a' is coprime to 26
    if (gcd(a, 26) != 1) {
        printf("Error: 'a' must be coprime to 26 for a valid encryption.\n");
        return 1;
    }

    encrypt_message(plaintext, a, b);

    return 0;
}
