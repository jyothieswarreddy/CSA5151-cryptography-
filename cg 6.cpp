#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define ALPHABET_SIZE 26

int gcd(int a, int b) {
    if (b == 0)
        return a;
    return gcd(b, a % b);
}

int modInverse(int a, int m) {
    a = a % m;
    for (int x = 1; x < m; x++)
        if ((a * x) % m == 1)
            return x;
    return -1;
}

int main() {
    char ciphertext[] = "YOUR_CIPHERTEXT_HERE";
    int freq[ALPHABET_SIZE] = {0};
    int n = strlen(ciphertext);

    // Count the frequency of each letter
    for (int i = 0; i < n; i++) {
        if (isalpha(ciphertext[i])) {
            int index = toupper(ciphertext[i]) - 'A';
            freq[index]++;
        }
    }

    // Find the most frequent letter
    int max_freq = 0;
    int max_index = 0;
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        if (freq[i] > max_freq) {
            max_freq = freq[i];
            max_index = i;
        }
    }

    // Find the second most frequent letter
    int second_max_freq = 0;
    int second_max_index = 0;
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        if (i != max_index && freq[i] > second_max_freq) {
            second_max_freq = freq[i];
            second_max_index = i;
        }
    }

    // Assume that the most frequent letter in the plaintext is 'E'
    // and the second most frequent letter is 'T'
    int e_index = 4;
    int t_index = 19;

    // Solve for a and b using the equations:
    // e_index = a * max_index + b (mod 26)
    // t_index = a * second_max_index + b (mod 26)
    int a, b;
    for (a = 1; a < ALPHABET_SIZE; a++) {
        if (gcd(a, ALPHABET_SIZE) == 1) {
            b = (e_index - a * max_index) % ALPHABET_SIZE;
            if (b < 0)
                b += ALPHABET_SIZE;
            if ((a * second_max_index + b) % ALPHABET_SIZE == t_index)
                break;
        }
    }

    // Decrypt the ciphertext
    printf("Plaintext: ");
    for (int i = 0; i < n; i++) {
        if (isalpha(ciphertext[i])) {
            int index = toupper(ciphertext[i]) - 'A';
            int plaintext_index = (modInverse(a, ALPHABET_SIZE) * (index - b)) % ALPHABET_SIZE;
            if (plaintext_index < 0)
                plaintext_index += ALPHABET_SIZE;
            printf("%c", plaintext_index + 'A');
        } else {
            printf("%c", ciphertext[i]);
        }
    }
    printf("\n");

    return 0;
}
