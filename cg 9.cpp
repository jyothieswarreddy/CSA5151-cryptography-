#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define KEY "ROYALNEWZEALANDNAVY"
#define ALPHABET "ABCDEFGHIJKLMNOPQRSTUVWXYZ"

char playfair_matrix[5][5];

void build_matrix() {
    int i, j, k = 0;
    for (i = 0; i < 5; i++) {
        for (j = 0; j < 5; j++) {
            playfair_matrix[i][j] = KEY[k++];
            if (k >= strlen(KEY)) break;
        }
        if (k >= strlen(KEY)) break;
    }
}

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

void decrypt(char *ciphertext, char *plaintext) {
    int i, j, row1, col1, row2, col2;
    for (i = 0; i < strlen(ciphertext); i += 2) {
        row1 = get_row(ciphertext[i]);
        col1 = get_col(ciphertext[i]);
        row2 = get_row(ciphertext[i + 1]);
        col2 = get_col(ciphertext[i + 1]);
        if (row1 == row2) {
            plaintext[i] = playfair_matrix[row1][(col1 - 1 + 5) % 5];
            plaintext[i + 1] = playfair_matrix[row2][(col2 - 1 + 5) % 5];
        } else if (col1 == col2) {
            plaintext[i] = playfair_matrix[(row1 - 1 + 5) % 5][col1];
            plaintext[i + 1] = playfair_matrix[(row2 - 1 + 5) % 5][col2];
        } else {
            plaintext[i] = playfair_matrix[row1][col2];
            plaintext[i + 1] = playfair_matrix[row2][col1];
        }
    }
    plaintext[i] = '\0';
}

int main() {
    char ciphertext[] = "KXJEYUREBEZVVEHEWRYTUHEYFSKREHEGOYFIWTTTTUOLKSYCAJPOBOTEIZONTXBYBNTGONEYCUZWRGDSONSXBOUYVRHEBAAHYUSEDQ";
    char plaintext[strlen(ciphertext) + 1];
    build_matrix();
    decrypt(ciphertext, plaintext);
    printf("Decrypted message: %s\n", plaintext);
    return 0;
}
