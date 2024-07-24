#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define SIZE 5

void prepareMatrix(char key[], char matrix[SIZE][SIZE]) {
    int alphabet[26] = {0};
    int x = 0, y = 0;

    // Fill the matrix with the key
    for (int i = 0; i < strlen(key); i++) {
        if (key[i] == 'J') key[i] = 'I';  // Treat J as I
        if (alphabet[key[i] - 'A'] == 0) {
            matrix[x][y++] = key[i];
            alphabet[key[i] - 'A'] = 1;
            if (y == SIZE) {
                y = 0;
                x++;
            }
        }
    }

    // Fill the remaining matrix with other letters
    for (char c = 'A'; c <= 'Z'; c++) {
        if (c == 'J') continue;  // Skip J
        if (alphabet[c - 'A'] == 0) {
            matrix[x][y++] = c;
            alphabet[c - 'A'] = 1;
            if (y == SIZE) {
                y = 0;
                x++;
            }
        }
    }
}

void printMatrix(char matrix[SIZE][SIZE]) {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            printf("%c ", matrix[i][j]);
        }
        printf("\n");
    }
}

void preprocessText(char text[], char processed[]) {
    int len = strlen(text);
    int index = 0;

    for (int i = 0; i < len; i++) {
        if (text[i] == 'J') text[i] = 'I';  // Treat J as I
        if (isalpha(text[i])) {
            processed[index++] = toupper(text[i]);
        }
    }
    processed[index] = '\0';

    // Make the text length even by adding 'X' if needed
    if (index % 2 != 0) {
        processed[index++] = 'X';
        processed[index] = '\0';
    }
}

void findPosition(char matrix[SIZE][SIZE], char ch, int *row, int *col) {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (matrix[i][j] == ch) {
                *row = i;
                *col = j;
                return;
            }
        }
    }
}

void encryptPair(char matrix[SIZE][SIZE], char a, char b, char *c1, char *c2) {
    int row1, col1, row2, col2;
    findPosition(matrix, a, &row1, &col1);
    findPosition(matrix, b, &row2, &col2);

    if (row1 == row2) {
        *c1 = matrix[row1][(col1 + 1) % SIZE];
        *c2 = matrix[row2][(col2 + 1) % SIZE];
    } else if (col1 == col2) {
        *c1 = matrix[(row1 + 1) % SIZE][col1];
        *c2 = matrix[(row2 + 1) % SIZE][col2];
    } else {
        *c1 = matrix[row1][col2];
        *c2 = matrix[row2][col1];
    }
}

void encryptText(char matrix[SIZE][SIZE], char text[], char encrypted[]) {
    int len = strlen(text);
    int index = 0;

    for (int i = 0; i < len; i += 2) {
        char a = text[i];
        char b = (i + 1 < len) ? text[i + 1] : 'X';
        if (a == b) b = 'X';  // Replace same letters in a pair with 'X'

        char c1, c2;
        encryptPair(matrix, a, b, &c1, &c2);

        encrypted[index++] = c1;
        encrypted[index++] = c2;
    }
    encrypted[index] = '\0';
}

int main() {
    char key[100], text[100], processedText[100], encryptedText[100];
    char matrix[SIZE][SIZE];

    // Input the key
    printf("Enter the keyword: ");
    fgets(key, sizeof(key), stdin);
    key[strcspn(key, "\n")] = '\0';  // Remove the newline character

    // Prepare the Playfair matrix
    prepareMatrix(key, matrix);

    // Display the Playfair matrix
    printf("Playfair Matrix:\n");
    printMatrix(matrix);

    // Input the text to be encrypted
    printf("Enter the text to be encrypted: ");
    fgets(text, sizeof(text), stdin);
    text[strcspn(text, "\n")] = '\0';  // Remove the newline character

    // Preprocess the text
    preprocessText(text, processedText);

    // Encrypt the text
    encryptText(matrix, processedText, encryptedText);

    // Output the encrypted text
    printf("Encrypted text: %s\n", encryptedText);

    return 0;
}

