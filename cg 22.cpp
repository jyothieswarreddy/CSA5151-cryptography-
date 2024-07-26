#include <stdio.h>
#include <string.h>

#define IP(input) (((input & 0x80) >> 3) | ((input & 0x40) >> 1) | ((input & 0x20) >> 1) | ((input & 0x10) << 1) | \
                   ((input & 0x08) << 3) | ((input & 0x04) >> 1) | ((input & 0x02) << 1) | ((input & 0x01) << 3))

#define IP_INV(input) (((input & 0x80) >> 3) | ((input & 0x40) >> 1) | ((input & 0x20) >> 3) | ((input & 0x10) << 1) | \
                       ((input & 0x08) << 1) | ((input & 0x04) << 3) | ((input & 0x02) << 1) | ((input & 0x01) << 3))

#define SWAP_NIBBLES(input) (((input & 0xF0) >> 4) | ((input & 0x0F) << 4))

unsigned char sbox[2][4][4] = {
    { {1, 0, 3, 2}, {3, 2, 1, 0}, {0, 2, 1, 3}, {3, 1, 3, 2} },
    { {0, 1, 2, 3}, {2, 0, 1, 3}, {3, 0, 1, 0}, {2, 1, 0, 3} }
};

unsigned char p4(unsigned char input) {
    return ((input & 0x8) >> 3) | ((input & 0x4) >> 1) | ((input & 0x2) << 1) | ((input & 0x1) << 3);
}

unsigned char fk(unsigned char input, unsigned char sk) {
    unsigned char left = (input & 0xF0) >> 4;
    unsigned char right = input & 0x0F;
    
    unsigned char ep = ((right & 0x08) >> 3) | ((right & 0x04) << 1) | ((right & 0x02) >> 1) | ((right & 0x01) << 3) | \
                      ((right & 0x08) >> 1) | ((right & 0x04) >> 1) | ((right & 0x02) << 3) | ((right & 0x01) << 1);
                      
    unsigned char x = ep ^ sk;
    unsigned char s0 = sbox[0][(x & 0x80) >> 6 | (x & 0x20) >> 5][(x & 0x40) >> 4 | (x & 0x10) >> 3];
    unsigned char s1 = sbox[1][(x & 0x08) >> 2 | (x & 0x02) >> 1][(x & 0x04) >> 1 | (x & 0x01)];
    unsigned char s = (s0 << 2) | s1;
    
    return ((left ^ p4(s)) << 4) | right;
}

void keygen(unsigned char key, unsigned char *k1, unsigned char *k2) {
    unsigned char p10 = ((key & 0x200) >> 6) | ((key & 0x100) >> 3) | ((key & 0x080) >> 1) | ((key & 0x040) >> 2) | ((key & 0x020) >> 1) | \
                        ((key & 0x010) << 3) | ((key & 0x008) >> 2) | ((key & 0x004) << 2) | ((key & 0x002) << 1) | ((key & 0x001) << 4);
                        
    unsigned char ls1 = ((p10 & 0x1F0) >> 4) | ((p10 & 0x00F) << 4);
    unsigned char ls1_5bit = ((ls1 & 0x1F) << 1) | ((ls1 & 0x10) >> 4);
    unsigned char ls2 = ((p10 & 0x00F) << 5) | ((p10 & 0x1F0) >> 1);
    
    *k1 = ((ls1_5bit & 0x1E) << 2) | ((ls1 & 0x20) >> 4) | ((ls1_5bit & 0x1C) >> 3) | ((ls1_5bit & 0x02) << 1);
    *k2 = ((ls2 & 0x1E) << 2) | ((ls2 & 0x20) >> 4) | ((ls2 & 0x1C) >> 3) | ((ls2 & 0x02) << 1);
}

unsigned char encrypt(unsigned char plaintext, unsigned char key) {
    unsigned char k1, k2;
    keygen(key, &k1, &k2);
    
    unsigned char ip = IP(plaintext);
    unsigned char fk1 = fk(ip, k1);
    unsigned char sw = SWAP_NIBBLES(fk1);
    unsigned char fk2 = fk(sw, k2);
    unsigned char ciphertext = IP_INV(fk2);
    
    return ciphertext;
}

unsigned char decrypt(unsigned char ciphertext, unsigned char key) {
    unsigned char k1, k2;
    keygen(key, &k1, &k2);
    
    unsigned char ip = IP(ciphertext);
    unsigned char fk2 = fk(ip, k2);
    unsigned char sw = SWAP_NIBBLES(fk2);
    unsigned char fk1 = fk(sw, k1);
    unsigned char plaintext = IP_INV(fk1);
    
    return plaintext;
}

void cbc_encrypt(unsigned char *plaintext, unsigned char *ciphertext, int length, unsigned char key, unsigned char iv) {
    unsigned char prev_block = iv;
    
    for (int i = 0; i < length; i++) {
        plaintext[i] ^= prev_block;
        ciphertext[i] = encrypt(plaintext[i], key);
        prev_block = ciphertext[i];
    }
}

void cbc_decrypt(unsigned char *ciphertext, unsigned char *plaintext, int length, unsigned char key, unsigned char iv) {
    unsigned char prev_block = iv;
    
    for (int i = 0; i < length; i++) {
        unsigned char decrypted_block = decrypt(ciphertext[i], key);
        plaintext[i] = decrypted_block ^ prev_block;
        prev_block = ciphertext[i];
    }
}

int main() {
    unsigned char iv = 0b10101010;
    unsigned char key = 0b0111111101;
    unsigned char plaintext[] = { 0b00000001, 0b00100011 };
    unsigned char ciphertext[sizeof(plaintext)];
    unsigned char decryptedtext[sizeof(plaintext)];

    int length = sizeof(plaintext);

    cbc_encrypt(plaintext, ciphertext, length, key, iv);

    printf("Ciphertext: ");
    for (int i = 0; i < length; i++) {
        printf("%02x ", ciphertext[i]);
    }
    printf("\n");
    cbc_decrypt(ciphertext, decryptedtext, length, key, iv);

    printf("Decrypted text: ");
    for (int i = 0; i < length; i++) {
        printf("%02x ", decryptedtext[i]);
    }
    printf("\n");

    return 0;
}

