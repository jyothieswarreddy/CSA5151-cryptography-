#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define ALPHABET_SIZE 26
#define MAX_TEXT_SIZE 1000
#define TOP_PLAINTEXTS 10

// Frequency of letters in the English language (approximate)
const double english_letter_frequency[ALPHABET_SIZE] = {
    8.167, 1.492, 2.782, 4.253, 12.702, 2.228, 2.015, 6.094, 6.966, 0.153,
    0.747, 4.025, 2.406, 6.749, 7.507, 1.929, 0.095, 5.987, 6.327, 9.056,
    2.758, 0.978, 2.360, 0.150, 1.974, 0.074
};

// Structure to hold plaintext and its score
typedef struct {
    char text[MAX_TEXT_SIZE];
    double score;
} Plaintext;

// Function to count the frequency of letters in a given text
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

// Function to compare two plaintexts based on their scores
int compare_plaintext_scores(const void *a, const void *b) {
    Plaintext *p1 = (Plaintext *)a;
    Plaintext *p2 = (Plaintext *)b;
    return (p1->score > p2->score) ? -1 : 1;
}

// Function to compute a score for a given text based on letter frequency
double compute_score(const char *text, const int *frequency) {
    double score = 0.0;
    int total_letters = 0;

    for (int i = 0; i < ALPHABET_SIZE; i++) {
        total_letters += frequency[i];
    }

    if (total_letters == 0) {
        return score;
    }

    for (int i = 0; i < ALPHABET_SIZE; i++) {
        double observed_frequency = (double)frequency[i] / total_letters * 100;
        score += (english_letter_frequency[i] - observed_frequency) * (english_letter_frequency[i] - observed_frequency);
    }

    return score;
}

// Function to generate possible plaintexts by trying different substitutions
void generate_possible_plaintexts(const char *ciphertext, Plaintext *plaintexts, int *ciphertext_frequency) {
    int sorted_indices[ALPHABET_SIZE];

    for (int i = 0; i < ALPHABET_SIZE; i++) {
        sorted_indices[i] = i;
    }

    // Sort indices based on frequency
    for (int i = 0; i < ALPHABET_SIZE - 1; i++) {
        for (int j = i + 1; j < ALPHABET_SIZE; j++) {
            if (ciphertext_frequency[sorted_indices[i]] < ciphertext_frequency[sorted_indices[j]]) {
                int temp = sorted_indices[i];
                sorted_indices[i] = sorted_indices[j];
                sorted_indices[j] = temp;
            }
        }
    }

    for (int i = 0; i < ALPHABET_SIZE; i++) {
        char substitution_table[ALPHABET_SIZE];

        for (int j = 0; j < ALPHABET_SIZE; j++) {
            substitution_table[sorted_indices[j]] = 'A' + j;
        }

        char possible_plaintext[MAX_TEXT_SIZE];
        strcpy(possible_plaintext, ciphertext);

        for (int j = 0; possible_plaintext[j] != '\0'; j++) {
            if (isalpha(possible_plaintext[j])) {
                char original_char = toupper(possible_plaintext[j]);
                int index = original_char - 'A';
                possible_plaintext[j] = substitution_table[index];
            }
        }

        plaintexts[i].score = compute_score(possible_plaintext, ciphertext_frequency);
        strcpy(plaintexts[i].text, possible_plaintext);
    }
}

int main() {
    char ciphertext[MAX_TEXT_SIZE];
    Plaintext plaintexts[ALPHABET_SIZE];

    printf("Enter the ciphertext: ");
    fgets(ciphertext, sizeof(ciphertext), stdin);
    ciphertext[strcspn(ciphertext, "\n")] = 0; // Remove newline character

    int ciphertext_frequency[ALPHABET_SIZE];
    count_letter_frequency(ciphertext, ciphertext_frequency);

    generate_possible_plaintexts(ciphertext, plaintexts, ciphertext_frequency);

    // Sort plaintexts by score
    qsort(plaintexts, ALPHABET_SIZE, sizeof(Plaintext), compare_plaintext_scores);

    printf("\nTop %d possible plaintexts:\n", TOP_PLAINTEXTS);
    for (int i = 0; i < TOP_PLAINTEXTS && i < ALPHABET_SIZE; i++) {
        printf("Plaintext %d: %s (Score: %.2f)\n", i + 1, plaintexts[i].text, plaintexts[i].score);
    }

    return 0;
}

