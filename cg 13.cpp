#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MOD 26
#define MATRIX_SIZE 2

// Function to multiply two matrices
void multiplyMatrices(int a[MATRIX_SIZE][MATRIX_SIZE], int b[MATRIX_SIZE], int result[MATRIX_SIZE]) {
    for (int i = 0; i < MATRIX_SIZE; i++) {
        result[i] = 0;
        for (int j = 0; j < MATRIX_SIZE; j++) {
            result[i] += a[i][j] * b[j];
        }
        result[i] %= MOD;
    }
}

// Function to find the modular inverse of a number
int modInverse(int a, int m) {
    a = a % m;
    for (int x = 1; x < m; x++) {
        if ((a * x) % m == 1) {
            return x;
        }
    }
    return -1; // Inverse doesn't exist
}

// Function to calculate the determinant of the key matrix
int determinant(int key[MATRIX_SIZE][MATRIX_SIZE]) {
    return (key[0][0] * key[1][1] - key[0][1] * key[1][0]) % MOD;
}

// Function to find the inverse of the key matrix
int inverseMatrix(int key[MATRIX_SIZE][MATRIX_SIZE], int inverse[MATRIX_SIZE][MATRIX_SIZE]) {
    int det = determinant(key);
    int detInv = modInverse(det, MOD);
    if (detInv == -1) return -1; // Inverse doesn't exist

    inverse[0][0] = (key[1][1] * detInv) % MOD;
    inverse[0][1] = (-key[0][1] * detInv) % MOD;
    inverse[1][0] = (-key[1][0] * detInv) % MOD;
    inverse[1][1] = (key[0][0] * detInv) % MOD;

    // Adjust negative values to be positive
    for (int i = 0; i < MATRIX_SIZE; i++) {
        for (int j = 0; j < MATRIX_SIZE; j++) {
            if (inverse[i][j] < 0) {
                inverse[i][j] += MOD;
            }
        }
    }
    return 0;
}

// Function to encrypt the plaintext
void encrypt(char *plaintext, int key[MATRIX_SIZE][MATRIX_SIZE], char *ciphertext) {
    int len = strlen(plaintext);
    int vector[MATRIX_SIZE];
    int result[MATRIX_SIZE];
    int index = 0;

    for (int i = 0; i < len; i += MATRIX_SIZE) {
        for (int j = 0; j < MATRIX_SIZE; j++) {
            if (i + j < len) {
                vector[j] = plaintext[i + j] - 'a'; // Convert char to number
            } else {
                vector[j] = 0; // Padding with 'a' (0)
            }
        }
        multiplyMatrices(key, vector, result);
        for (int j = 0; j < MATRIX_SIZE; j++) {
            ciphertext[index++] = result[j] + 'a'; // Convert back to char
        }
    }
    ciphertext[index] = '\0'; // Null terminate the string
}

// Function to decrypt the ciphertext
void decrypt(char *ciphertext, int key[MATRIX_SIZE][MATRIX_SIZE], char *plaintext) {
    int len = strlen(ciphertext);
    int vector[MATRIX_SIZE];
    int result[MATRIX_SIZE];
    int inverse[MATRIX_SIZE][MATRIX_SIZE];
    inverseMatrix(key, inverse);
    
    int index = 0;

    for (int i = 0; i < len; i += MATRIX_SIZE) {
        for (int j = 0; j < MATRIX_SIZE; j++) {
            vector[j] = ciphertext[i + j] - 'a'; // Convert char to number
        }
        multiplyMatrices(inverse, vector, result);
        for (int j = 0; j < MATRIX_SIZE; j++) {
            plaintext[index++] = result[j] + 'a'; // Convert back to char
        }
    }
    plaintext[index] = '\0'; // Null terminate the string
}

// Function to recover the key from known plaintext and ciphertext
void recoverKey(char *plaintext, char *ciphertext, int key[MATRIX_SIZE][MATRIX_SIZE]) {
    int p[MATRIX_SIZE][MATRIX_SIZE];
    int c[MATRIX_SIZE][MATRIX_SIZE];
    int vectorP[MATRIX_SIZE];
    int vectorC[MATRIX_SIZE];

    // Fill matrices with known plaintext and ciphertext
    for (int i = 0; i < MATRIX_SIZE; i++) {
        vectorP[i] = plaintext[i] - 'a';
        vectorC[i] = ciphertext[i] - 'a';
    }

    // Create the key matrix
    for (int i = 0; i < MATRIX_SIZE; i++) {
        for (int j = 0; j < MATRIX_SIZE; j++) {
            p[i][j] = vectorP[j]; // Fill with plaintext vector
            c[i][j] = vectorC[j]; // Fill with ciphertext vector
        }
    }

    // Calculate the key matrix K = C * P^(-1)
    int inverseP[MATRIX_SIZE][MATRIX_SIZE];
    inverseMatrix(p, inverseP);

    // Multiply C by P^(-1) to get K
    for (int i = 0; i < MATRIX_SIZE; i++) {
        for (int j = 0; j < MATRIX_SIZE; j++) {
            key[i][j] = 0;
            for (int k = 0; k < MATRIX_SIZE; k++) {
                key[i][j] += c[i][k] * inverseP[k][j];
            }
            key[i][j] %= MOD;
        }
    }
}

int main() {
    char plaintext[] = "me";
    char ciphertext[100];
    char decrypted[100];
    
    // Example key for encryption
    int key[MATRIX_SIZE][MATRIX_SIZE] = {
        {9, 4},
        {5, 7}
    };

    // Encrypt the known plaintext
    encrypt(plaintext, key, ciphertext);
    printf("Known Plaintext: %s\n", plaintext);
    printf("Ciphertext: %s\n", ciphertext);
    
    // Recover the key using known plaintext and ciphertext
    int recoveredKey[MATRIX_SIZE][MATRIX_SIZE];
    recoverKey(plaintext, ciphertext, recoveredKey);
    
    // Display the recovered key
    printf("Recovered Key:\n");
    for (int i = 0; i < MATRIX_SIZE; i++) {
        for (int j = 0; j < MATRIX_SIZE; j++) {
            printf("%d ", recoveredKey[i][j]);
        }
        printf("\n");
    }

    // Validate the recovered key by decrypting the ciphertext
    decrypt(ciphertext, recoveredKey, decrypted);
    printf("Decrypted with Recovered Key: %s\n", decrypted);

    return 0;
}
