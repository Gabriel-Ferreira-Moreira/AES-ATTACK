#include <stdio.h>
#include <stdint.h>
#include "aes.h"
#include <stdlib.h> 
#include "convert.h"

int main(void) 
{ 
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif

        char frases[][50] = {"ola mundo novo!!"};
        int numFrases = sizeof(frases) / sizeof(frases[0]);
        uint8_t input[16];
        char output[17]; // 16 caracteres + terminador '\0'
        uint8_t rest[16] = {0};

        
        // Converte as strings para o array de bytes
        convertStringArrayToHex(frases, numFrases, input);


        uint8_t key[16] = { 
        0x2b,0x7e,0x15,0x16, 
        0x28,0xae,0xd2,0xa6,
        0xab,0xf7,0x15,0x88, 
        0x09,0xcf,0x4f,0x3c };

    struct AES_ctx ctx;
        AES_init_ctx(&ctx, key); //RoundKeys
        

        printf("\nKey: ");
        for (int i = 0; i < 16; i++) {
            printf("%02x", key[i]);
        }
        
        printf("\nInput: %s\n", frases[0]);
        #if 0
        printf("Input Hexa: ");
        for (int i = 0; i < 16; i++) {
            printf("%02x ", input[i]);
        }
        printf("\n");
        #endif


        // Encripta
        AES_ECB_encrypt(&ctx, input);

        convertHexToString(input, output); 
        printf("Encrypt: %s\n", output);
        #if 0
        printf("Encrypt: ");
        for (int i = 0; i < 16; i++) {
            printf("%02x ", input[i]);
        }
        printf("\n");
        #endif

        // Decripta de volta
        AES_ECB_decrypt(&ctx, input);
        convertHexToString(input, output); 

        printf("Decrypt: %s\n", output);
        #if 0
        printf("Decrypt Hexa: ");
        for (int i = 0; i < 16; i++) {
            printf("%02x ", input[i]);
        }
        #endif
        printf("\n");

    return 0;
}


