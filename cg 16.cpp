#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define ALPHABET_SIZE 26
#define MAX_PLAINTEXTS 10
#define MAX_CIPHERTEXT_LENGTH 1000

// Expected frequency of letters in English
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

// Function to calculate the frequency of letters in the ciphertext
void calculate_frequency(char *ciphertext, int *frequency) {
    for (int i = 0; ciphertext[i] != '\0'; i++) {
        if (isalpha(ciphertext[i])) {
            char letter = tolower(ciphertext[i]);
            frequency[letter - 'a']++;
        }
    }
}

// Function to generate a substitution key based on frequency analysis
void generate_substitution_key(int *frequency, char *substitution_key) {
    int sorted_indices[ALPHABET_SIZE];
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        sorted_indices[i] = i;
    }

    // Sort indices based on frequency
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        for (int j = i + 1; j < ALPHABET_SIZE; j++) {
            if (frequency[sorted_indices[j]] > frequency[sorted_indices[i]]) {
                int temp = sorted_indices[i];
                sorted_indices[i] = sorted_indices[j];
                sorted_indices[j] = temp;
            }
        }
    }

    // Map sorted frequencies to English letters
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        substitution_key[sorted_indices[i]] = 'a' + i; // Map to 'a', 'b', 'c', ...
    }
}

// Function to decrypt the ciphertext using the substitution key
void decrypt_with_key(char *ciphertext, char *substitution_key, char *plaintext) {
    for (int i = 0; ciphertext[i] != '\0'; i++) {
        if (isalpha(ciphertext[i])) {
            char base = islower(ciphertext[i]) ? 'a' : 'A';
            plaintext[i] = substitution_key[tolower(ciphertext[i]) - 'a'];
            if (isupper(ciphertext[i])) {
                plaintext[i] = toupper(plaintext[i]);
            }
        } else {
            plaintext[i] = ciphertext[i]; // Keep non-alpha characters unchanged
        }
    }
    plaintext[strlen(ciphertext)] = '\0'; // Null terminate the string
}

// Function to perform the frequency attack
void frequency_attack(char *ciphertext, int top_n) {
    int frequency[ALPHABET_SIZE] = {0};
    char substitution_key[ALPHABET_SIZE] = {0};
    char plaintext[MAX_CIPHERTEXT_LENGTH];

    // Calculate letter frequency in the ciphertext
    calculate_frequency(ciphertext, frequency);

    // Generate a substitution key based on frequency analysis
    generate_substitution_key(frequency, substitution_key);

    // Decrypt the ciphertext using the generated substitution key
    decrypt_with_key(ciphertext, substitution_key, plaintext);

    // Display the top N possible plaintexts (in this case, just one)
    printf("Possible plaintext (based on frequency analysis):\n");
    printf("%s\n", plaintext);
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
