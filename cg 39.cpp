#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define ALPHABET_SIZE 26
#define MAX_TEXT_LENGTH 1000

// English letter frequency order (from most frequent to least frequent)
const double english_letter_frequency[ALPHABET_SIZE] = {
    0.12702, // A
    0.09056, // B
    0.08167, // C
    0.07507, // D
    0.06966, // E
    0.06749, // F
    0.06327, // G
    0.06094, // H
    0.05987, // I
    0.04253, // J
    0.04025, // K
    0.02782, // L
    0.02560, // M
    0.02406, // N
    0.02360, // O
    0.02228, // P
    0.02015, // Q
    0.01974, // R
    0.01929, // S
    0.01492, // T
    0.00978, // U
    0.00772, // V
    0.00153, // W
    0.00150, // X
    0.00095, // Y
    0.00074  // Z
};

// Function to calculate the chi-squared statistic for a given shift
double calculate_chi_squared(const char *text, int shift) {
    int letter_count[ALPHABET_SIZE] = {0};
    int total_letters = 0;
    double chi_squared = 0.0;

    // Count letter frequencies for this shift
    for (int i = 0; text[i] != '\0'; i++) {
        if (isalpha(text[i])) {
            char shifted_letter = toupper(text[i]) - 'A';
            shifted_letter = (shifted_letter - shift + ALPHABET_SIZE) % ALPHABET_SIZE; // Shift and wrap
            letter_count[shifted_letter]++;
            total_letters++;
        }
    }

    // Calculate chi-squared statistic
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        double observed = letter_count[i];
        double expected = total_letters * english_letter_frequency[i];
        chi_squared += ((observed - expected) * (observed - expected)) / expected;
    }

    return chi_squared;
}

// Function to decrypt text using a given shift
void decrypt_with_shift(const char *ciphertext, int shift, char *plaintext) {
    for (int i = 0; ciphertext[i] != '\0'; i++) {
        if (isalpha(ciphertext[i])) {
            char shifted_letter = toupper(ciphertext[i]) - 'A';
            shifted_letter = (shifted_letter - shift + ALPHABET_SIZE) % ALPHABET_SIZE; // Shift and wrap
            plaintext[i] = shifted_letter + 'A';
        } else {
            plaintext[i] = ciphertext[i];
        }
    }
    plaintext[strlen(ciphertext)] = '\0';
}

// Function to perform letter frequency attack
void letter_frequency_attack(const char *ciphertext, int top_n) {
    double chi_squared_values[ALPHABET_SIZE];
    int top_shifts[top_n];
    char possible_plaintexts[top_n][MAX_TEXT_LENGTH];

    // Calculate chi-squared for each shift
    for (int shift = 0; shift < ALPHABET_SIZE; shift++) {
        chi_squared_values[shift] = calculate_chi_squared(ciphertext, shift);
    }

    // Find top N shifts with lowest chi-squared values
    for (int n = 0; n < top_n; n++) {
        double min_chi_squared = chi_squared_values[0];
        int best_shift = 0;
        for (int shift = 1; shift < ALPHABET_SIZE; shift++) {
            if (chi_squared_values[shift] < min_chi_squared) {
                min_chi_squared = chi_squared_values[shift];
                best_shift = shift;
            }
        }
        top_shifts[n] = best_shift;
        chi_squared_values[best_shift] = 1e9; // Mark this shift as used
        decrypt_with_shift(ciphertext, best_shift, possible_plaintexts[n]);
    }

    // Output top N possible plaintexts
    for (int i = 0; i < top_n; i++) {
        printf("Possible plaintext %d (shift %d): %s\n", i + 1, top_shifts[i], possible_plaintexts[i]);
    }
}

int main() {
    char ciphertext[MAX_TEXT_LENGTH];
    int top_n;

    printf("Enter the ciphertext: ");
    fgets(ciphertext, sizeof(ciphertext), stdin);
    ciphertext[strcspn(ciphertext, "\n")] = '\0'; // Remove newline character

    printf("Enter the number of top possible plaintexts to display: ");
    scanf("%d", &top_n);

    if (top_n > ALPHABET_SIZE) {
        printf("Number of possible plaintexts requested exceeds maximum shifts available. Using %d instead.\n", ALPHABET_SIZE);
        top_n = ALPHABET_SIZE;
    }

    letter_frequency_attack(ciphertext, top_n);

    return 0;
}

