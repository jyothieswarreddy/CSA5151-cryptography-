#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX_MESSAGE_LENGTH 1000

int main() {
    char ciphertext[] = "53‡‡†305))6*;4826)4‡.)4‡);806*;48†8¶60))85;;]8*;:‡*8†83(88)5*†;46(;88*96*?;8)*‡(;485);5*†2:*‡(;4956*2(5*—4)8¶8*;4069285);)6†8)4‡‡;1(‡9;48081;8:8‡1;48†85;4)485†528806*81 (‡9;48;(88;4(‡?34;48)4‡;161;:188;‡?;";
    char plaintext[MAX_MESSAGE_LENGTH];
    int i;

    // Frequency analysis to find the most common characters
    int freq[128] = {0};
    for (i = 0; i < strlen(ciphertext); i++) {
        freq[(int)ciphertext[i]]++;
    }

    int max_freq = 0;
    char max_char = 0;
    for (i = 0; i < 128; i++) {
        if (freq[i] > max_freq) {
            max_freq = freq[i];
            max_char = (char)i;
        }
    }

    // Assume the most common character represents 'e'
    char e_char = max_char;

    // Assume the second most common character represents 't'
    char t_char = 0;
    max_freq = 0;
    for (i = 0; i < 128; i++) {
        if (freq[i] > max_freq && (char)i != e_char) {
            max_freq = freq[i];
            t_char = (char)i;
        }
    }

    // Assume the third most common character represents 'h'
    char h_char = 0;
    max_freq = 0;
    for (i = 0; i < 128; i++) {
        if (freq[i] > max_freq && (char)i != e_char && (char)i != t_char) {
            max_freq = freq[i];
            h_char = (char)i;
        }
    }

    // Decrypt the ciphertext
    for (i = 0; i < strlen(ciphertext); i++) {
        if (ciphertext[i] == e_char) {
            plaintext[i] = 'e';
        } else if (ciphertext[i] == t_char) {
            plaintext[i] = 't';
        } else if (ciphertext[i] == h_char) {
            plaintext[i] = 'h';
        } else {
            plaintext[i] = ciphertext[i];
        }
    }
    plaintext[i] = '\0';

    printf("Plaintext: %s\n", plaintext);

    return 0;
}
