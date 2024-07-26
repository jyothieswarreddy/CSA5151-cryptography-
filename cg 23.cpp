#include <stdio.h>

unsigned char encrypt(unsigned char plaintext, unsigned char key);
unsigned char decrypt(unsigned char ciphertext, unsigned char key);

void keygen(unsigned char key, unsigned char *k1, unsigned char *k2);

void ctr_encrypt(unsigned char *plaintext, unsigned char *ciphertext, int length, unsigned char key, unsigned char counter);
void ctr_decrypt(unsigned char *ciphertext, unsigned char *plaintext, int length, unsigned char key, unsigned char counter);

int main() {
    unsigned char counter = 0b00000000;
    unsigned char key = 0b0111111101;
    unsigned char plaintext[] = { 0b00000001, 0b00000010, 0b00000100 };
    unsigned char ciphertext[sizeof(plaintext)];
    unsigned char decryptedtext[sizeof(plaintext)];
    int length = sizeof(plaintext) / sizeof(plaintext[0]);

    ctr_encrypt(plaintext, ciphertext, length, key, counter);

    printf("Ciphertext: ");
    for (int i = 0; i < length; i++) {
        printf("%02x ", ciphertext[i]);
    }
    printf("\n");

    ctr_decrypt(ciphertext, decryptedtext, length, key, counter);

    printf("Decrypted text: ");
    for (int i = 0; i < length; i++) {
        printf("%02x ", decryptedtext[i]);
    }
    printf("\n");

    return 0;
}

void keygen(unsigned char key, unsigned char *k1, unsigned char *k2) {

}

unsigned char fk(unsigned char input, unsigned char sk) {
    return input; 
}

unsigned char encrypt(unsigned char plaintext, unsigned char key) {
    unsigned char k1, k2;
    keygen(key, &k1, &k2);
    unsigned char result = fk(plaintext, k1); 
    return result;
}

unsigned char decrypt(unsigned char ciphertext, unsigned char key) {
    unsigned char k1, k2;
    keygen(key, &k1, &k2);
    unsigned char result = fk(ciphertext, k2);
    return result;
}

void ctr_encrypt(unsigned char *plaintext, unsigned char *ciphertext, int length, unsigned char key, unsigned char counter) {
    for (int i = 0; i < length; i++) {
        unsigned char keystream = encrypt(counter, key);
        ciphertext[i] = plaintext[i] ^ keystream;
        counter++;
    }
}

void ctr_decrypt(unsigned char *ciphertext, unsigned char *plaintext, int length, unsigned char key, unsigned char counter) {
    for (int i = 0; i < length; i++) {
        unsigned char keystream = encrypt(counter, key);
        plaintext[i] = ciphertext[i] ^ keystream;
        counter++;
    }
}

