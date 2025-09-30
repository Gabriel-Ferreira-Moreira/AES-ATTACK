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
    for (int rodada = 1; rodada <= 3; rodada++) {
        printf("\n===== Rodada %d =====\n\n", rodada);

        #ifdef _WIN32
            system("cls");
        #else
            //system("clear");
        #endif

        struct AES_ctx ctx;
        uint8_t key[16] = { 
            0x60,0x3d,0xeb,0x10, 
            0x15,0xca,0x71,0xbe, 
            0x2b,0x73,0xae,0xf0, 
            0x85,0x7d,0x77,0x81 
        };

        AES_init_ctx(&ctx, key);

        uint8_t input[16] = {0}, posicao[16] = {0};
        int conts = 0, contador = 0;
        AES_ECB_encrypt(&ctx, input, &conts);
        printf("Acessos na memória lenta (chave inicial): %d\n\n", conts);

        uint8_t inputs[16][16];
        gerar_inputs(inputs);

        for (int i = 0; i < 16; i++) {
            contador = 0;  // zera a cada chamada
            AES_ECB_encrypt(&ctx, inputs[i], &contador);

            printf("Input %d -> Acessos na memória lenta: %d\n", i, contador - 74);
        }
    }

    return 0;
}
