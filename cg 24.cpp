#include <stdio.h>
#include <stdlib.h>

// Extended Euclidean algorithm to find the multiplicative inverse
long long int extended_gcd(long long int a, long long int b, long long int *x, long long int *y) {
    if (b == 0) {
        *x = 1;
        *y = 0;
        return a;
    }
    
    long long int x1, y1;
    long long int gcd = extended_gcd(b, a % b, &x1, &y1);
    
    *x = y1;
    *y = x1 - (a / b) * y1;
    
    return gcd;
}

// Function to find the private key (d)
long long int find_private_key(long long int e, long long int phi) {
    long long int x, y;
    long long int gcd = extended_gcd(e, phi, &x, &y);
    
    if (gcd != 1) {
        printf("Error: e and phi are not coprime.\n");
        return -1;
    }
    
    // Ensure d is positive
    long long int d = (x + phi) % phi;
    return d;
}

int main() {
    long long int e = 31;
    long long int n = 3599;
    
    // Find p and q using trial-and-error
    long long int p, q;
    for (p = 2; p * p <= n; p++) {
        if (n % p == 0) {
            q = n / p;
            break;
        }
    }
    
    printf("p = %lld\n", p);
    printf("q = %lld\n", q);
    
    // Calculate phi(n)
    long long int phi = (p - 1) * (q - 1);
    
    // Find the private key (d)
    long long int d = find_private_key(e, phi);
    
    if (d != -1) {
        printf("Private key (d) = %lld\n", d);
    }
    
    return 0;
}
