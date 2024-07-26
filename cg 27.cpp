#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Function to simulate RSA encryption of a single character
int rsa_encrypt(int plaintext, int e, int n) {
    // Simple modular exponentiation for encryption
    long long int ciphertext = 1;
    for (int i = 0; i < e; i++) {
        ciphertext = (ciphertext * plaintext) % n;
    }
    return (int)ciphertext;
}

// Function to simulate RSA decryption of a single character
int rsa_decrypt(int ciphertext, int d, int n) {
    // Simple modular exponentiation for decryption
    long long int plaintext = 1;
    for (int i = 0; i < d; i++) {
        plaintext = (plaintext * ciphertext) % n;
    }
    return (int)plaintext;
}

int main() {
    // Example RSA parameters (small for demonstration, should be large in practice)
    int e = 7;  // Public exponent
    int d = 3;  // Private exponent (not realistic for large n)
    int n = 55; // Modulus (not realistic for large n)

    // Message to encrypt (A-Z represented as 0-25)
    char message[] = "HELLO";
    int length = sizeof(message) - 1; // Exclude null terminator

    // Encrypt each character
    printf("Original Message: %s\n", message);
    printf("Encrypted Message:\n");
    for (int i = 0; i < length; i++) {
        int plaintext = message[i] - 'A'; // Convert 'A' to 0, 'B' to 1, etc.
        int ciphertext = rsa_encrypt(plaintext, e, n);
        printf("Character: %c, Ciphertext: %d\n", message[i], ciphertext);
    }

    // Note: Decryption is not shown here as the focus is on encryption vulnerabilities.
    
    return 0;
}
