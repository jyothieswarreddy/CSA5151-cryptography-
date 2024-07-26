#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// Function to calculate modular exponentiation
long long int mod_exp(long long int base, long long int exponent, long long int modulus) {
    long long int result = 1;
    while (exponent > 0) {
        if (exponent % 2 == 1) {
            result = (result * base) % modulus;
        }
        base = (base * base) % modulus;
        exponent /= 2;
    }
    return result;
}

int main() {
    long long int q = 23;  // A large prime number
    long long int a = 5;   // A primitive root modulo q
    
    // Alice's secret number
    long long int x = 6;
    
    // Bob's secret number
    long long int y = 15;
    
    // Correct Diffie-Hellman key exchange
    long long int alice_public = mod_exp(a, x, q);
    long long int bob_public = mod_exp(a, y, q);
    
    long long int alice_key = mod_exp(bob_public, x, q);
    long long int bob_key = mod_exp(alice_public, y, q);
    
    printf("Correct Diffie-Hellman:\n");
    printf("Alice's public value: %lld\n", alice_public);
    printf("Bob's public value: %lld\n", bob_public);
    printf("Shared key: %lld\n", alice_key);
    
    // Incorrect Diffie-Hellman key exchange (sending xa instead of a^x mod q)
    alice_public = mod_exp(a, x, q);
    bob_public = mod_exp(a, y, q);
    
    alice_key = mod_exp(bob_public, x, q);
    bob_key = mod_exp(alice_public, y, q);
    
    printf("\nIncorrect Diffie-Hellman:\n");
    printf("Alice's public value: %lld\n", alice_public);
    printf("Bob's public value: %lld\n", bob_public);
    printf("Shared key: %lld\n", alice_key);
    
    return 0;
}
