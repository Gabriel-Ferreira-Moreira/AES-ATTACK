// Ataca todos os rounds com o Oráculo 2 informando apenas a quantidade de vezes em que a barreira de memória foi ultrapassada
// Defina o RDN e o BR em include/aes.h 

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "aes.h"
#include "convert.h"

#define ITERS 1000
#define BLOCK_SIZE 16

void extrair_MSB(const uint8_t key[16], uint8_t MSB[16]) {
    for (int i = 0; i < 16; i++) {
        // Pega o bit mais significativo e normaliza para 0 ou 1
        MSB[i] = (key[i] & 0x80) >> 7;
    }
}

int main(void) {
    struct AES_ctx ctx;
    uint8_t key[16] = {0x60, 0x3d, 0xeb, 0x10, 0x15, 0xca, 0x71, 0xbe, 0x2b, 0x73, 0xae, 0xf0, 0x85, 0x7d, 0x77, 0x81};

    AES_init_ctx(&ctx, key);

    uint8_t input[16] = {0};
    int conts;
    double sum_zero, sum_msb[16], avg_msb[16];
    double avg_zero;
    int bits_encontrados[16] = {0};
    uint8_t MSB[16];

    extrair_MSB(key, MSB); // Extrai os MSBs da chave

    srand(time(NULL));
// Loop para descobrir 1 bit por byte
for (int pos = 0; pos < 16; pos++) {
    sum_zero = 0;               // soma para o caso com MSB[0..pos] = 0
    sum_msb[pos] = 0;           // soma para o caso com MSB[pos] = 1

    /* ======= 1) ITERS vezes com MSB[0..pos] = 0 (resto aleatório) ======= */
    for (int it = 0; it < ITERS; it++) {
        conts = 0;
        for (int j = 0; j < 16; j++)
            input[j] = rand() & 0xFF; // todos aleatórios

        // força MSB = 0 para todos os bytes de 0 até pos (inclusive)
        for (int b = 0; b < pos; b++) {
            input[b] &= 0x7F; // 0111 1111
        }

        AES_ECB_encrypt(&ctx, input, &conts);
        sum_zero += conts;
    }

    /* ======= 2) ITERS vezes com MSB[pos] = 1 e bytes anteriores fixos ======= */
    for (int it = 0; it < ITERS; it++) {
        conts = 0;
        for (int j = 0; j < 16; j++)
            input[j] = rand() & 0xFF; // todos aleatórios

        for (int b = 0; b < pos; b++) {
        input[b] &= 0x7F; // 1000 0000
        }

        // força MSB atual (pos) = 1
        input[pos] |= 0x80;

        AES_ECB_encrypt(&ctx, input, &conts);
        sum_msb[pos] += conts;
    }

    /* ======= cálculo das médias e decisão ======= */
    avg_msb[pos] = (double)sum_msb[pos] / (double)ITERS;
    avg_zero = (double)sum_zero / (double)ITERS;

    //printf("\n--- POSIÇÃO %d ---\n", pos);
    //printf("avg_msb[%d] = %.6f | avg_zero = %.6f\n", pos, avg_msb[pos], avg_zero);

    if ((avg_msb[pos] - avg_zero) > 0) {
        bits_encontrados[pos] = 0;
        //printf("Bit[%d] = 0 (avg_msb > avg_zero)\n", pos);
    } else {
        bits_encontrados[pos] = 1;
        //printf("Bit[%d] = 1 (avg_msb <= avg_zero)\n", pos);
    }
}

    /* Compara bits_encontrados e MSB extraído */
    for (int pos = 0; pos < 16; pos++) {
        if (bits_encontrados[pos] == MSB[pos])
            printf("pos %2d: recovered == MSB (both %u) -> OK\n", pos, bits_encontrados[pos]);
        else
            printf("pos %2d: recovered=%u != MSB=%u -> ERROR\n", pos, bits_encontrados[pos], MSB[pos]);
    }


    return 0;
}
