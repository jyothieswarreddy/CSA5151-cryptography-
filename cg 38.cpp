#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MATRIX_SIZE 3 // Size of the Hill cipher key matrix (3x3)
#define MODULO 26     // Number of letters in the English alphabet

// Function to calculate the determinant of a 3x3 matrix
int determinant(int matrix[MATRIX_SIZE][MATRIX_SIZE]) {
    int det = 0;
    for (int i = 0; i < MATRIX_SIZE; i++) {
        det += matrix[0][i] * (matrix[1][(i + 1) % MATRIX_SIZE] * matrix[2][(i + 2) % MATRIX_SIZE] -
                               matrix[1][(i + 2) % MATRIX_SIZE] * matrix[2][(i + 1) % MATRIX_SIZE]);
    }
    return det % MODULO;
}

// Function to find the modular inverse of a number a under modulo m using extended Euclidean algorithm
int mod_inverse(int a, int m) {
    a = a % m;
    for (int x = 1; x < m; x++) {
        if ((a * x) % m == 1) {
            return x;
        }
    }
    return -1; // Inverse doesn't exist
}

// Function to calculate the cofactor matrix for a given matrix
void cofactor_matrix(int matrix[MATRIX_SIZE][MATRIX_SIZE], int cofactor[MATRIX_SIZE][MATRIX_SIZE]) {
    int temp[MATRIX_SIZE - 1][MATRIX_SIZE - 1];
    for (int row = 0; row < MATRIX_SIZE; row++) {
        for (int col = 0; col < MATRIX_SIZE; col++) {
            int p = 0, q = 0;
            for (int i = 0; i < MATRIX_SIZE; i++) {
                for (int j = 0; j < MATRIX_SIZE; j++) {
                    if (i != row && j != col) {
                        temp[p][q++] = matrix[i][j];
                        if (q == MATRIX_SIZE - 1) {
                            q = 0;
                            p++;
                        }
                    }
                }
            }
            cofactor[row][col] = ((temp[0][0] * temp[1][1]) - (temp[0][1] * temp[1][0])) * ((row + col) % 2 == 0 ? 1 : -1);
            cofactor[row][col] = (cofactor[row][col] % MODULO + MODULO) % MODULO;
        }
    }
}

// Function to calculate the inverse of a matrix modulo 26
int inverse_matrix(int matrix[MATRIX_SIZE][MATRIX_SIZE], int inverse[MATRIX_SIZE][MATRIX_SIZE]) {
    int det = determinant(matrix);
    int det_inv = mod_inverse(det, MODULO);
    if (det_inv == -1) {
        return 0; // Inverse does not exist
    }
    int cofactor[MATRIX_SIZE][MATRIX_SIZE];
    cofactor_matrix(matrix, cofactor);
    // Transpose of cofactor matrix to get adjugate
    for (int i = 0; i < MATRIX_SIZE; i++) {
        for (int j = 0; j < MATRIX_SIZE; j++) {
            inverse[i][j] = (cofactor[j][i] * det_inv) % MODULO;
            if (inverse[i][j] < 0) {
                inverse[i][j] += MODULO;
            }
        }
    }
    return 1; // Inverse exists
}

// Function to multiply two matrices and store result
void multiply_matrices(int a[MATRIX_SIZE][MATRIX_SIZE], int b[MATRIX_SIZE][MATRIX_SIZE], int result[MATRIX_SIZE][MATRIX_SIZE]) {
    for (int i = 0; i < MATRIX_SIZE; i++) {
        for (int j = 0; j < MATRIX_SIZE; j++) {
            result[i][j] = 0;
            for (int k = 0; k < MATRIX_SIZE; k++) {
                result[i][j] += a[i][k] * b[k][j];
            }
            result[i][j] %= MODULO;
        }
    }
}

// Function to convert a block of text into a matrix
void text_to_matrix(const char *text, int matrix[MATRIX_SIZE][MATRIX_SIZE], int block_size) {
    for (int i = 0; i < MATRIX_SIZE; i++) {
        for (int j = 0; j < block_size; j++) {
            matrix[i][j] = text[i * block_size + j] - 'A';
        }
    }
}

// Function to convert a matrix into text
void matrix_to_text(int matrix[MATRIX_SIZE][MATRIX_SIZE], char *text, int block_size) {
    for (int i = 0; i < MATRIX_SIZE; i++) {
        for (int j = 0; j < block_size; j++) {
            text[i * block_size + j] = matrix[i][j] + 'A';
        }
    }
}

// Function to perform known plaintext attack on Hill cipher
void known_plaintext_attack(const char *plaintext, const char *ciphertext, int block_size) {
    int plaintext_matrix[MATRIX_SIZE][MATRIX_SIZE];
    int ciphertext_matrix[MATRIX_SIZE][MATRIX_SIZE];
    int plaintext_inverse[MATRIX_SIZE][MATRIX_SIZE];
    int key_matrix[MATRIX_SIZE][MATRIX_SIZE];

    // Convert plaintext and ciphertext into matrices
    text_to_matrix(plaintext, plaintext_matrix, block_size);
    text_to_matrix(ciphertext, ciphertext_matrix, block_size);

    // Calculate the inverse of the plaintext matrix
    if (!inverse_matrix(plaintext_matrix, plaintext_inverse)) {
        printf("Inverse of plaintext matrix does not exist.\n");
        return;
    }

    // Calculate the key matrix
    multiply_matrices(plaintext_inverse, ciphertext_matrix, key_matrix);

    // Display the key matrix
    printf("Key matrix obtained from known plaintext attack:\n");
    for (int i = 0; i < MATRIX_SIZE; i++) {
        for (int j = 0; j < MATRIX_SIZE; j++) {
            printf("%d ", key_matrix[i][j]);
        }
        printf("\n");
    }
}

int main() {
    // Example known plaintext-ciphertext pair
    char plaintext[] = "ACT";
    char ciphertext[] = "POH";

    printf("Known plaintext: %s\n", plaintext);
    printf("Ciphertext: %s\n", ciphertext);

    // Perform the attack
    known_plaintext_attack(plaintext, ciphertext, strlen(plaintext));

    return 0;
}

