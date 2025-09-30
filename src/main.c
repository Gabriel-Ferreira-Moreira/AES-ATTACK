#include <stdio.h>
#include <stdint.h>
#include "aes.h"
#include <stdlib.h> 
#include "convert.h"
#include <string.h>

#define ITERS 10000
int main(void) 
{ 

        #ifdef _WIN32
            system("cls");
        #else
            system("clear");
        #endif

        struct AES_ctx ctx;
        uint8_t key[16] = { 
            0x60,0x3d,0xeb,0x10, 
            0x15,0xca,0x71,0xbe, 
            0x2b,0x73,0xae,0xf0, 
            0x85,0x7d,0x77,0x81 
        };

        AES_init_ctx(&ctx, key);

        uint8_t input[16];
        int conts;

        long long sum_zero = 0;
        long long sum_msb  = 0;

        // 1) ITERS vezes com input todo zero
        for (int it = 0; it < ITERS; it++) {
            conts = 0;
            memset(input, 0, 16); // zera tudo
            AES_ECB_encrypt(&ctx, input, &conts);
            sum_zero += conts;
        }

        // 2) ITERS vezes com input[0] = 0x80 e resto zero
        for (int it = 0; it < ITERS; it++) {
            conts = 0;
            memset(input, 0, 16); // zera tudo
            input[0] = 0x80;
            AES_ECB_encrypt(&ctx, input, &conts);
            sum_msb += conts;
        }

        // 3) Printar as médias
        double avg_zero = (double)sum_zero / (double)ITERS;
        double avg_msb  = (double)sum_msb  / (double)ITERS;

        printf("Média (input todo zero) : %.2f\n", avg_zero);
        printf("Média (input[0]=0x80)   : %.2f\n", avg_msb);

    return 0;
}
