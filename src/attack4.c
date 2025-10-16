// Ataca todos os rounds com o Oráculo 2 informando apenas a quantidade de vezes em que a barreira de memória foi ultrapassada
// Defina o RDN e o BR em include/aes.h 
// INCOMPLETO

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "aes.h"
#include "convert.h"

#define ITERS 1000

// Pega o bit mais significativo de cada byte e armazena em MSB (esquerda -> direita)
void extrair_MSB(const uint8_t key[16], uint8_t MSB[16]) {
    for (int i = 0; i < 16; i++) {
        MSB[i] = (key[i] & 0x80) >> 7;
    }
}

int main(int argc, char *argv[]) {
    struct AES_ctx ctx;
    uint8_t key[16] = {0x60, 0x3d, 0xeb, 0x10, 0x15, 0xca, 0x71, 0xbe, 0x2b, 0x73, 0xae, 0xf0, 0x85, 0x7d, 0x77, 0x81};

    AES_init_ctx(&ctx, key);

    uint8_t input[16] = {0};
    int conts;
    double sum_zero, sum_msb[16], avg_msb[16];
    double avg_zero; 
    int Bit[16] = {0}; // Armazena os bits encontrados da chave
    uint8_t MSB[16]; // Armazena os bits mais significativos da chave 

    extrair_MSB(key, MSB); // Extrai os MSBs da chave

    srand(time(NULL)); // Semente
    // int seed = atoi(argv[1]);
    // srand(seed);

    // Ordem em que as S-boxes são acessados pela implementação. O ataque abaixo é montado descobrindo os bits mais
    // significativos da chave exatamente nessa ordem.
    
    // No caso do código original do arquivo aes.c, o acesso é feito por linhas, mas o estado do AES é armazenado por
    // colunas. Por isso, é usada a ordem a seguir.
    const int ordem[] = { 0, 4, 8, 12, 1, 5, 9, 13, 2, 6, 10, 14, 3, 7, 11, 15 };
    // A ordem natural, caso as S-boxes fossem acessadas por colunas, seria:
    // const int ordem[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };
    // Adicione aqui a ordem da implementação em assembly, quando descobrir qual é.

// Loop para descobrir 1 bit por byte
for (int pos = 0; pos < 16; pos++) {
    sum_zero = 0;               // soma para o caso com MSB[0..pos] = 0
    sum_msb[ordem[pos]] = 0;           // soma para o caso com MSB[pos] = 1

    /* ======= 1) ITERS vezes com MSB[0..pos] = 0 (resto aleatório) ======= */
    for (int it = 0; it < ITERS; it++) {
        conts = 0;
        for (int j = 0; j < 16; j++)
            input[j] = rand() & 0xFF; // todos aleatórios

        // força MSB = 0 para todos os bytes de 0 até pos 
        for (int b = 0; b < pos; b++) {
            // Será montado um ataque não-adaptativo ("em malha aberta").
            input[ordem[b]] &= 0x7F; // 0111 1111

            // Caso deseje fazer o ataque adaptativo ("em malha fechada"), as linhas abaixo devem ser descomentadas
            // if (Bit[ordem[b]] == 1) {
            //     input[ordem[b]] |= 0x80; // 1000 0000
            // }
        }

        input[ordem[pos]] &= 0x7F; // força MSB atual (pos) = 0

        AES_ECB_encrypt(&ctx, input, &conts);
        sum_zero += conts;
    }

    /* ======= 2) ITERS vezes com MSB[pos] = 1 e bytes anteriores fixos ======= */
    for (int it = 0; it < ITERS; it++) {
        conts = 0;
        for (int j = 0; j < 16; j++)
            input[j] = rand() & 0xFF; // todos aleatórios
        
        #if 1
        // Substitue os bits anteriores (0..pos) por 1
        for (int b = 0; b < pos; b++) {
            // Ver comentários acima sobre ataque adaptativo ou não-adaptativo
            input[ordem[b]] &= 0x7F; // 0111 1111

            // if (Bit[ordem[b]] == 1) {
            //     input[ordem[b]] |= 0x80; // 1000 0000
            // }
        }

        #else // Substitue os bits anteriores (0..pos-1) conforme o valor encontrado
        if (Bit[pos] == 0) {
            for (int b = 0; b < pos; b++) 
                input[b] &= 0x7F; // 0111 1111

        } else { // Bit[pos] == 1
            for (int b = 0; b < pos; b++) 
                input[b] |= 0x80; // 1000 0000
            }
        #endif

        // força MSB atual (pos) = 1
        input[ordem[pos]] |= 0x80;

        AES_ECB_encrypt(&ctx, input, &conts);
        sum_msb[ordem[pos]] += conts;
    }

    /* ======= cálculo das médias e decisão ======= */
    avg_msb[ordem[pos]] = (double)sum_msb[ordem[pos]] / (double)ITERS;
    avg_zero = (double)sum_zero / (double)ITERS;

    // printf("\n--- POSIÇÃO %d ---\n", pos);
    // printf("avg_msb[%d] = %.6f | avg_zero = %.6f\n", pos, avg_msb[pos], avg_zero);

    if ((avg_msb[ordem[pos]] - avg_zero) > 0) {
        Bit[ordem[pos]] = 0;
        // printf("Bit[%d] = 0 (avg_msb > avg_zero)\n", pos);
    } else {
        Bit[ordem[pos]] = 1;
        // printf("Bit[%d] = 1 (avg_msb <= avg_zero)\n", pos);
    } 

    // As linhas a seguir precisam ser executadas em caso de um ataque não-adaptativo. Para um ataque adaptativo, elas
    // devem ser comentadas.
    if (pos > 0 && Bit[ordem[pos - 1]] == 1) {
        Bit[ordem[pos]] = 1 - Bit[ordem[pos]];
    }


    // printf("Pos = %d - %s\n", pos, (Bit[pos] == MSB[pos]) ? "OK" : "ERRO");
    // printf("sum_msb  = %d\n", (int)sum_msb[pos]);
    // printf("sum_zero = %d\n\n", (int)sum_zero);
}
    int correct = 0;
    /* Compara bits_encontrados e MSB extraído */
    for (int pos = 0; pos < 16; pos++) {
        if (Bit[pos] == MSB[pos]){
            printf("pos %2d: Bit: %u -> OK\n", pos, Bit[pos]);
            correct++;
        }
        else
            printf("pos %2d: Bit: %u -> ERROR\n", pos, Bit[pos]);
    }
    printf("\nTotal de bits corretos: %d/16\n", correct);


    return 0;
}
