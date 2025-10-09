// Ataca o 1 round com o Oráculo X informando a ordem q em foi acessada a memória lenta
// Defina o RDN em include/aes.h 
// INCOMPLETO

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
    int conts = 0;
    AES_ECB_encrypt(&ctx, input, &conts);



    return 0;
}
