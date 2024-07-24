#include <stdio.h>
#include <string.h>
#include <ctype.h>

char playfair_matrix[5][5] = {
    {'M', 'F', 'H', 'I', 'K'},
    {'U', 'N', 'O', 'P', 'Q'},
    {'Z', 'V', 'W', 'X', 'Y'},
    {'E', 'L', 'A', 'R', 'G'},
    {'D', 'S', 'T', 'B', 'C'}
};

int get_row(char c) {
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            if (playfair_matrix[i][j] == c) return i;
        }
    }
    return -1;
}

int get_col(char c) {
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            if (playfair_matrix[i][j] == c) return j;
        }
    }
    return -1;
}

void encrypt(char *plaintext, char *ciphertext) {
    int i, j, row1, col1, row2, col2;
    for (i = 0; i < strlen(plaintext); i += 2) {
        row1 = get_row(plaintext[i]);
        col1 = get_col(plaintext[i]);
        row2 = get_row(plaintext[i + 1]);
        col2 = get_col(plaintext[i + 1]);
        if (row1 == row2) {
            ciphertext[i] = playfair_matrix[row1][(col1 + 1) % 5];
            ciphertext[i + 1] = playfair_matrix[row2][(col2 + 1) % 5];
        } else if (col1 == col2) {
            ciphertext[i] = playfair_matrix[(row1 + 1) % 5][col1];
            ciphertext[i + 1] = playfair_matrix[(row2 + 1) % 5][col2];
        } else {
            ciphertext[i] = playfair_matrix[row1][col2];
            ciphertext[i + 1] = playfair_matrix[row2][col1];
        }
    }
    ciphertext[i] = '\0';
}

int main() {
    char plaintext[] = "MUSTSEEYYOUCADOGANWESTCOMINGATONCE";
    char ciphertext[strlen(plaintext) + 1];
    encrypt(plaintext, ciphertext);
    printf("Encrypted message: %s\n", ciphertext);
    return 0;
}
