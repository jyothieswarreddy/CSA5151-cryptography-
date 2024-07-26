#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

// Function to calculate GCD
long long gcd(long long a, long long b) {
    if (b == 0) return a;
    return gcd(b, a % b);
}

// Function to find modular inverse using Extended Euclidean Algorithm
long long mod_inverse(long long a, long long m) {
    long long m0 = m, t, q;
    long long x0 = 0, x1 = 1;

    if (m == 1) return 0;

    while (a > 1) {
        q = a / m;
        t = m;

        m = a % m;
        a = t;
        t = x0;

        x0 = x1 - q * x0;
        x1 = t;
    }

    if (x1 < 0) x1 += m0;

    return x1;
}

// Function to generate a random prime number (simple implementation)
long long generate_prime() {
    long long num;
    while (1) {
        num = rand() % 100 + 50; // Random number between 50 and 149
        int is_prime = 1;
        for (long long i = 2; i <= sqrt(num); i++) {
            if (num % i == 0) {
                is_prime = 0;
                break;
            }
        }
        if (is_prime) return num;
    }
}

// Function to generate RSA keys
void generate_keys(long long *n, long long *e, long long *d) {
    long long p = generate_prime();
    long long q = generate_prime();
    *n = p * q;
    long long phi = (p - 1) * (q - 1);

    // Choose e
    *e = 3; // Common choice for e
    while (gcd(*e, phi) != 1) {
        (*e)++;
    }

    // Calculate d
    *d = mod_inverse(*e, phi);
}

int main() {
    srand(time(NULL)); // Seed for random number generation

    long long n, e, d;
    generate_keys(&n, &e, &d);

    printf("Public Key: (n = %lld, e = %lld)\n", n, e);
    printf("Private Key: d = %lld\n", d);

    // Simulate the leak of Bob's private key
    printf("\nBob's private key has been leaked!\n");
    
    // Generate new keys
    printf("Generating new keys...\n");
    generate_keys(&n, &e, &d);
    printf("New Public Key: (n = %lld, e = %lld)\n", n, e);
    printf("New Private Key: d = %lld\n", d);

    return 0;
}
