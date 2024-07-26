#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#define BLOCK_SIZE 1024  // Block size in bits
#define CAPACITY_BITS 512 // Capacity size in bits (arbitrary for the example)
#define RATE_BITS (BLOCK_SIZE - CAPACITY_BITS) // Rate size in bits

#define LANE_SIZE 64  // Lane size in bits
#define NUM_LANES (BLOCK_SIZE / LANE_SIZE) // Number of lanes

void initialize_state(uint64_t state[NUM_LANES]) {
    for (int i = 0; i < NUM_LANES; i++) {
        state[i] = 0;
    }
}

void initialize_message_block(uint64_t message_block[NUM_LANES]) {
    for (int i = 0; i < NUM_LANES; i++) {
        message_block[i] = rand(); // Randomly initialize to have at least one nonzero bit
    }
}

void absorb(uint64_t state[NUM_LANES], uint64_t message_block[NUM_LANES]) {
    for (int i = 0; i < RATE_BITS / LANE_SIZE; i++) {
        state[i] ^= message_block[i];
    }
}

int count_zero_capacity_lanes(uint64_t state[NUM_LANES]) {
    int zero_count = 0;
    for (int i = RATE_BITS / LANE_SIZE; i < NUM_LANES; i++) {
        if (state[i] == 0) {
            zero_count++;
        }
    }
    return zero_count;
}

void print_state(uint64_t state[NUM_LANES]) {
    for (int i = 0; i < NUM_LANES; i++) {
        printf("Lane %d: %016llx\n", i, state[i]);
    }
}

int main() {
    uint64_t state[NUM_LANES];
    uint64_t message_block[NUM_LANES];

    initialize_state(state);
    initialize_message_block(message_block);

    printf("Initial state:\n");
    print_state(state);

    absorb(state, message_block);

    printf("\nState after absorbing the first message block:\n");
    print_state(state);

    int iterations = 0;
    while (count_zero_capacity_lanes(state) > 0) {
        initialize_message_block(message_block); // Create a new message block
        absorb(state, message_block);
        iterations++;
    }

    printf("\nFinal state:\n");
    print_state(state);
    printf("\nNumber of iterations to fill capacity lanes: %d\n", iterations);

    return 0;
}

