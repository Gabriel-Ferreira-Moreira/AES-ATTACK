// Gera 16 inputs de 16 bytes cada, todos zerados, mas com o MSB = 1 em apenas um byte diferente em cada input
// Depois imprimi os acessos à memória lenta para cada input
// Ataque no 1 round ou em todos os rounds (definido em include/aes.h pela variável RDN)

#include <stdio.h>
#include <stdint.h>
#include "aes.h"
#include <stdlib.h> 
#include "convert.h"
#include <string.h>

void gerar_inputs(uint8_t inputs[16][16]) {
    for (int i = 0; i < 16; i++) {
        memset(inputs[i], 0, 16); // zera tudo
        inputs[i][i] = 0x80;      // coloca MSB = 1 no byte i
    }
}

int main(void) 
{ 
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif

    struct AES_ctx ctx;
    uint8_t key[16] = { 0x2b,0x7e,0x15,0x16, 0x28,0xae,0xd2,0xa6,
                        0xab,0xf7,0x15,0x88, 0x09,0xcf,0x4f,0x3c };
    AES_init_ctx(&ctx, key);

    uint8_t input[16] = {0}, posicao[16] = {0};
    int conts = 0, contador = 0;
    AES_ECB_encrypt(&ctx, input, &conts);
    printf("Acessos na memória lenta: %d\n\n", conts);// Quantidade de MSB = 1 na chave

    uint8_t inputs[16][16];
    gerar_inputs(inputs);

    #if 0
        for (int i = 0; i < 16; i++) {
            contador = 0;  // zera a cada chamada
            AES_ECB_encrypt(&ctx, inputs[i], &contador);

            //printf("Input %d -> Acessos na memória lenta: %d\n", i, contador);
            if(contador < conts)
            {
                posicao[i] = 1;
            }
        }
        for (int i = 0; i < 16; i++) {
            printf("Byte %d: %dxxx xxxx\n", i+1, posicao[i]);
        }
    #else
            for (int i = 0; i < 16; i++) {
            contador = 0;  // zera a cada chamada
            AES_ECB_encrypt(&ctx, inputs[i], &contador);

            printf("Input %d -> Acessos na memória lenta: %d\n", i, contador);
        }

    #endif

    return 0;
}
