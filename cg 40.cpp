#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define ALPHABET_SIZE 26

// Known frequency order of English letters (E, T, A, O, I, N, S, H, R, D, L, C, U, M, W, F, G, Y, P, B, V, K, J, X, Q, Z)
char english_frequency[ALPHABET_SIZE] = {'E', 'T', 'A', 'O', 'I', 'N', 'S', 'H', 'R', 'D', 'L', 'C', 'U', 'M', 'W', 'F', 'G', 'Y', 'P', 'B', 'V', 'K', 'J', 'X', 'Q', 'Z'};

// Function to count letter frequency in the ciphertext
void count_letter_frequency(const char *text, int *frequency) {
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        frequency[i] = 0;
    }

    for (int i = 0; text[i] != '\0'; i++) {
        if (isalpha(text[i])) {
            frequency[toupper(text[i]) - 'A']++;
        }
    }
}

// Function to compare two frequency arrays
int compare_frequency(const void *a, const void *b) {
    return (*(int *)b) - (*(int *)a);
}

// Function to create a mapping of ciphertext letters to plaintext letters
void create_mapping(int *cipher_freq, char *mapping) {
    // Temporary array to hold sorted frequencies
    int sorted_freq[ALPHABET_SIZE];
    char temp_mapping[ALPHABET_SIZE];

    // Initialize mapping
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        temp_mapping[i] = i + 'A';
        sorted_freq[i] = cipher_freq[i];
    }

    // Sort frequencies
    qsort(sorted_freq, ALPHABET_SIZE, sizeof(int), compare_frequency);

    // Create mapping
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        int index = -1;
        for (int j = 0; j < ALPHABET_SIZE; j++) {
            if (cipher_freq[j] == sorted_freq[i]) {
                index = j;
                cipher_freq[j] = -1; // Prevent re-mapping of the same frequency
                break;
            }
        }
        mapping[index] = english_frequency[i];
    }
}

// Function to decrypt the text using the generated mapping
void decrypt_text(const char *ciphertext, const char *mapping, char *plaintext) {
    for (int i = 0; ciphertext[i] != '\0'; i++) {
        if (isalpha(ciphertext[i])) {
            char letter = toupper(ciphertext[i]);
            plaintext[i] = mapping[letter - 'A'];
        } else {
            plaintext[i] = ciphertext[i];
        }
    }
    plaintext[strlen(ciphertext)] = '\0';
}

// Function to perform letter frequency attack
void letter_frequency_attack(const char *ciphertext, int top_n) {
    int cipher_freq[ALPHABET_SIZE];
    char mapping[ALPHABET_SIZE];
    char plaintext[1000]; // Assuming a max length for the text

    // Count frequency of letters in the ciphertext
    count_letter_frequency(ciphertext, cipher_freq);

    // Create mapping of ciphertext to plaintext letters
    create_mapping(cipher_freq, mapping);

    // Generate and print top possible plaintexts
    for (int i = 0; i < top_n; i++) {
        decrypt_text(ciphertext, mapping, plaintext);
        printf("Possible plaintext %d: %s\n", i + 1, plaintext);

        // Shift mapping for next possible plaintext
        if (i < ALPHABET_SIZE - 1) {
            char temp = mapping[i];
            mapping[i] = mapping[i + 1];
            mapping[i + 1] = temp;
        }
    }
}

int main() {
    char ciphertext[1000];
    int top_n;

    printf("Enter the ciphertext: ");
    fgets(ciphertext, sizeof(ciphertext), stdin);
    ciphertext[strcspn(ciphertext, "\n")] = '\0'; // Remove newline character

    printf("Enter the number of top possible plaintexts to display: ");
    scanf("%d", &top_n);

    letter_frequency_attack(ciphertext, top_n);

    return 0;
}

