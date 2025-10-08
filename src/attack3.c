// Ataca todos os rounds com o Oráculo 1 informando apenas a quantidade de vezes que memória lenta foi acessada
// Defina o RDN e o BR em include/aes.h 

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "aes.h"
#include "convert.h"

#define ITERS 1000

void extrair_MSB(const uint8_t key[16], uint8_t MSB[16]) {
    for (int i = 0; i < 16; i++) {
        // Pega o bit mais significativo e normaliza para 0 ou 1
        MSB[i] = (key[i] & 0x80) >> 7;
    }
}

void gerar_inputs(uint8_t inputs[16][16]) {
    for (int i = 0; i < 16; i++) {
        memset(inputs[i], 0, 16); // zera tudo
        inputs[i][i] = 0x80;      // coloca MSB = 1 no byte i
    }
}

int main(void) {
    struct AES_ctx ctx;
    uint8_t key[16] = {0x60, 0x3d, 0xeb, 0x10, 0x15, 0xca, 0x71, 0xbe, 0x2b, 0x73, 0xae, 0xf0, 0x85, 0x7d, 0x77, 0x81};

    uint8_t MSB[16];
    uint8_t input[16];
    uint8_t inputs[16][16];
    double avg_msb[16] = {0};
    int conts;
    long long sum_zero = 0;
    long long sum_msb[16] = {0};
    uint8_t chave[16] = {0};

    gerar_inputs(inputs);
    extrair_MSB(key, MSB);

    AES_init_ctx(&ctx, key);
    srand(time(NULL));

    /* ==== CASO BASE: input todo zero ==== */
    for (int it = 0; it < ITERS; it++) {
        conts = 0;
        for (int j = 0; j < 16; j++) {
            input[j] = rand() & 0x7F; // MSB zerado
        }
        AES_ECB_encrypt(&ctx, input, &conts);
        sum_zero += conts;
    }

    double avg_zero = (double)sum_zero / (double)ITERS;

    /* ==== TESTE PARA CADA BYTE COM MSB = 1 ==== */
    for (int pos = 0; pos < 16; pos++) {
        sum_msb[pos] = 0; // zera o acumulador de cada posição

        for (int it = 0; it < ITERS; it++) {
            conts = 0;

            // gera bytes aleatórios (MSB sempre 0)
            for (int j = 0; j < 16; j++) {
                input[j] = rand() & 0x7F;
            }

            // ativa MSB só na posição "pos"
            input[pos] |= 0x80;

            AES_ECB_encrypt(&ctx, input, &conts);
            sum_msb[pos] += conts;
        }

        avg_msb[pos] = (double)sum_msb[pos] / (double)ITERS;
    }

    // ==== PRINT DAS MÉDIAS ==== 
    /*
        printf("\nMédia (input todo zero): %.6f\n", avg_zero);

    printf("Médias (MSB em cada byte): ");
    for (int i = 0; i < 16; i++) {
        printf("%.6f \n", avg_msb[i]);
    }
    printf("\n");

    // ==== PRINT DA CHAVE EXTRAÍDA E REAL ====
        printf("\nChave descoberta (bits MSB): ");
    for (int i = 0; i < 16; i++)
        printf("%u ", chave[i]);

    printf("\nChave real (bits MSB):       ");
    for (int i = 0; i < 16; i++)
        printf("%u ", MSB[i]);

    printf("\n");
    */

    /* ==== TESTE PARA DESCOBRIR A CHAVE ==== */
    for (int pos = 0; pos < 16; pos++) {
        double delta = avg_msb[pos] - avg_zero;
        if (delta > 0)
            chave[pos] = 0;
        else
            chave[pos] = 1;
    }

    /* Compara chave e MSB extraído */
    for (int pos = 0; pos < 16; pos++) {
        if (chave[pos] == MSB[pos])
            printf("pos %2d: chave == MSB (both %u) -> OK\n", pos, chave[pos]);
        else
            printf("pos %2d: chave=%u != MSB=%u -> ERROR\n", pos, chave[pos], MSB[pos]);
    }

    printf("\nChave descoberta no 1 Round (16 bits MSB): \n");
    for (int i = 0; i < 16; i++) printf("%u ", chave[i]);
    printf("\n");

    return 0;
}
