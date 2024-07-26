#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

// Function to generate a random k value
int generate_random_k() {
    return rand() % 100; // Simple random value for demonstration
}

// Function to sign a message
void sign_message(const char *message, int private_key, int *signature, int *k) {
    *k = generate_random_k(); // Generate a new k for each signature
    // Simple signature generation logic (not secure)
    *signature = (private_key * (*k)) % 100; // Example signature
}

int main() {
    srand(time(NULL)); // Seed for random number generation

    const char *message = "Hello, this is a test message.";
    int private_key = 42; // Example private key
    int signature1, signature2;
    int k1, k2;

    // Sign the message twice
    sign_message(message, private_key, &signature1, &k1);
    sign_message(message, private_key, &signature2, &k2);

    // Output the results
    printf("Message: %s\n", message);
    printf("Private Key: %d\n", private_key);
    printf("Signature 1: %d (k1: %d)\n", signature1, k1);
    printf("Signature 2: %d (k2: %d)\n", signature2, k2);

    // Check if signatures are different
    if (signature1 != signature2) {
        printf("The signatures are different, demonstrating the use of unique k values.\n");
    } else {
        printf("The signatures are the same, which should not happen with proper k values.\n");
    }

    return 0;
}
