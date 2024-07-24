#include <stdio.h>
#include <math.h>

// Function to calculate factorial of a number
double factorial(int n) {
    double result = 1.0;
    for (int i = 2; i <= n; i++) {
        result *= i;
    }
    return result;
}

int main() {
    // Number of letters in the Playfair cipher grid
    int num_letters = 25;

    // Calculate the factorial of 25
    double total_permutations = factorial(num_letters);

    // Calculate the logarithm (base 2) of the factorial
    double log2_total_permutations = log2(total_permutations);

    // Considering symmetries (approximate by a factor of 16)
    double unique_permutations = total_permutations / 16.0;
    double log2_unique_permutations = log2(unique_permutations);

    // Print the results
    printf("Total possible keys (approx): 2^%.2f\n", log2_total_permutations);
    printf("Effectively unique keys (approx): 2^%.2f\n", log2_unique_permutations);

    return 0;
}

