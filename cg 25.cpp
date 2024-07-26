#include <stdio.h>
#include <stdlib.h>

// Function to find the greatest common divisor (GCD) using Euclidean algorithm
long long int gcd(long long int a, long long int b) {
    if (b == 0) {
        return a;
    }
    return gcd(b, a % b);
}

// Function to factor n using a known common factor
void factor_n(long long int n, long long int common_factor) {
    long long int p = gcd(n, common_factor);
    long long int q = n / p;
    
    printf("p = %lld\n", p);
    printf("q = %lld\n", q);
}

int main() {
    long long int n = 3599;
    long long int e = 31;
    long long int ciphertext[] = {2345, 1234, 3456, 2789};
    long long int known_plaintext = 123;
    
    // Calculate the common factor
    long long int common_factor = gcd(n, known_plaintext);
    
    if (common_factor != 1) {
        printf("Common factor found: %lld\n", common_factor);
        factor_n(n, common_factor);
    } else {
        printf("No common factor found.\n");
    }
    
    // Decrypt the ciphertext blocks
    for (int i = 0; i < sizeof(ciphertext) / sizeof(ciphertext[0]); i++) {
        long long int plaintext = ciphertext[i];
        printf("Ciphertext: %lld, Plaintext: %lld\n", ciphertext[i], plaintext);
    }
    
    return 0;
}
