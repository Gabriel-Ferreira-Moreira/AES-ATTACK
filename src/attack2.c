// Ataca o 1 round com o Oráculo 1 informando apenas a quantidade de vezes que memória lenta foi acessada
// Defina o RDN e o BR em include/aes.h 

#include <stdio.h>
#include <stdint.h>
#include "aes.h"
#include <stdlib.h> 
#include "convert.h"
#include <string.h>

int main(void) 
{ 
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif

    struct AES_ctx ctx;
    uint8_t key[16] = {0x60, 0x3d, 0xeb, 0x10, 0x15, 0xca, 0x71, 0xbe, 0x2b, 0x73, 0xae, 0xf0, 0x85, 0x7d, 0x77, 0x81};

    AES_init_ctx(&ctx, key);

    uint8_t input[16] = {0};
    int conts1 = 0;
    AES_ECB_encrypt(&ctx, input, &conts1);
    printf("Acessos na memória lenta: %d\n\n", conts1);// Quantidade de MSB = 1 na chave

    input[0] = 0x80; // Primeiro input com MSB = 1 no byte 0
    int conts2 = 0;
    AES_ECB_encrypt(&ctx, input, &conts2);
    printf("Acessos na memória lenta: %d\n", conts2);// Quantidade
    if (conts2 > conts1) printf("\nMSB = 0 no byte 0 da chave\n\n");
    else printf("\nMSB = 1 no byte 0 da chave\n\n");

    return 0;
}
