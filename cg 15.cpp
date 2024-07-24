#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define ALPHABET_SIZE 26
#define MAX_PLAINTEXTS 10
#define MAX_CIPHERTEXT_LENGTH 1000

// Letter frequency in English (based on frequency analysis)
double english_freq[ALPHABET_SIZE] = {
    0.082, // A
    0.015, // B
    0.028, // C
    0.043, // D
    0.127, // E
    0.022, // F
    0.020, // G
    0.061, // H
    0.070, // I
    0.002, // J
    0.015, // K
    0.040, // L
    0.024, // M
    0.067, // N
    0.075, // O
    0.019, // P
    0.001, // Q
    0.060, // R
    0.063, // S
    0.091, // T
    0.028, // U
    0.010, // V
    0.024, // W
    0.001, // X
    0.020, // Y
    0.002  // Z
};

// Function to calculate the frequency score of a plaintext
double calculate_score(char *plaintext) {
    int letter_count[ALPHABET_SIZE] = {0};
    int total_letters = 0;

    // Count letter frequencies
    for (int i = 0; plaintext[i] != '\0'; i++) {
        if (isalpha(plaintext[i])) {
            char letter = tolower(plaintext[i]);
            letter_count[letter - 'a']++;
            total_letters++;
        }
    }

    // Calculate the score based on frequency
    double score = 0.0;
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        if (total_letters > 0) {
            double observed_freq = (double)letter_count[i] / total_letters;
            score += observed_freq * english_freq[i];
        }
    }
    return score;
}

// Function to decrypt the ciphertext using a specific shift
void decrypt_with_shift(char *ciphertext, int shift, char *plaintext) {
    for (int i = 0; ciphertext[i] != '\0'; i++) {
        if (isalpha(ciphertext[i])) {
            char base = islower(ciphertext[i]) ? 'a' : 'A';
            plaintext[i] = (ciphertext[i] - base - shift + ALPHABET_SIZE) % ALPHABET_SIZE + base;
        } else {
            plaintext[i] = ciphertext[i]; // Keep non-alpha characters unchanged
        }
    }
    plaintext[strlen(ciphertext)] = '\0'; // Null terminate the string
}

// Function to perform the frequency attack
void frequency_attack(char *ciphertext, int top_n) {
    char plaintext[MAX_CIPHERTEXT_LENGTH];
    double scores[ALPHABET_SIZE];
    char possible_plaintexts[ALPHABET_SIZE][MAX_CIPHERTEXT_LENGTH];

    // Decrypt the ciphertext with all possible shifts
    for (int shift = 0; shift < ALPHABET_SIZE; shift++) {
        decrypt_with_shift(ciphertext, shift, plaintext);
        scores[shift] = calculate_score(plaintext);
        strcpy(possible_plaintexts[shift], plaintext);
    }

    // Sort the possible plaintexts based on their scores
    for (int i = 0; i < top_n; i++) {
        for (int j = i + 1; j < ALPHABET_SIZE; j++) {
            if (scores[j] > scores[i]) {
                // Swap scores
                double temp_score = scores[i];
                scores[i] = scores[j];
                scores[j] = temp_score;

                // Swap plaintexts
                char temp_text[MAX_CIPHERTEXT_LENGTH];
                strcpy(temp_text, possible_plaintexts[i]);
                strcpy(possible_plaintexts[i], possible_plaintexts[j]);
                strcpy(possible_plaintexts[j], temp_text);
            }
        }
    }

    // Display the top N possible plaintexts
    printf("Top %d possible plaintexts:\n", top_n);
    for (int i = 0; i < top_n; i++) {
        printf("%d: %s (Score: %.5f)\n", i + 1, possible_plaintexts[i], scores[i]);
    }
}

int main() {
    char ciphertext[MAX_CIPHERTEXT_LENGTH];
    int top_n;

    // Input ciphertext
    printf("Enter the ciphertext: ");
    fgets(ciphertext, MAX_CIPHERTEXT_LENGTH, stdin);
    ciphertext[strcspn(ciphertext, "\n")] = 0; // Remove newline character

    // Input number of top plaintexts to display
    printf("Enter the number of top possible plaintexts to display: ");
    scanf("%d", &top_n);
    if (top_n > MAX_PLAINTEXTS) {
        top_n = MAX_PLAINTEXTS; // Limit to MAX_PLAINTEXTS
    }

    // Perform the frequency attack
    frequency_attack(ciphertext, top_n);

    return 0;
}
