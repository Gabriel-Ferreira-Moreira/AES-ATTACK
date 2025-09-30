#include "convert.h"
#include <string.h>  

#if STATE
void FirstBit(state_t* state) {
    for (int i = 0; i < 4; i++) 
    {
        for (int j = 0; j < 4; j++) 
        {
            uint8_t byte = (*state)[i][j];
            // Pega o primeiro bit (MSB = bit 7)
            uint8_t msb = (byte & 0x80) >> 7;

        #if 0
            // --- MODO 1: estilo matriz 4x4 ---
            printf("%dxxx-xxxx  ", msb);

            // quebra de linha a cada 4 bytes (como era 4x4)
            if ((i + 1) % 4 == 0) {
                printf("\n");
            }
            #else
            // --- MODO 2: linha única de bits ---
            printf("%d ", msb);
            #endif
        }
    }
}

void XorBlock(const uint8_t key[16], const uint8_t input[16], state_t state) {
    int k = 0;
    for (int col = 0; col < 4; col++) {
        for (int row = 0; row < 4; row++) {
            state[row][col] = key[k] ^ input[k];  // XOR e já coloca na posição correta
            k++;
        }
    }
}
#else
void FirstBit(uint8_t input[16]) {
    for (int i = 0; i < 16; i++) {
        uint8_t byte = input[i];
        // Pega o primeiro bit (MSB)
        uint8_t msb = (byte & 0x80) >> 7;

        #if 0
        // --- MODO 1: estilo matriz 4x4 ---
        printf("%dxxx-xxxx  ", msb);

        // quebra de linha a cada 4 bytes (como era 4x4)
        if ((i + 1) % 4 == 0) {
            printf("\n");
        }
        #else
        // --- MODO 2: linha única de bits ---
        printf("%d ", msb);
        #endif
    }
    printf("\n");

void XorBlock(const uint8_t key[16], const uint8_t input[16], uint8_t output[16]) {
for (int i = 0; i < 16; i++) {
    output[i] = key[i] ^ input[i];
                             }
}
}
#endif

void vectorToState(uint8_t output[16], state_t state) {
    int k = 0;
    for (int col = 0; col < 4; col++) {     
        for (int row = 0; row < 4; row++) { 
            state[row][col] = output[k++];
        }
    }
}

void convertStringArrayToHex(char frases[][50], int numFrases, uint8_t input[16]) {
    int index = 0;
    
    for (int i = 0; i < numFrases && index < 16; i++) {
        int len = strlen(frases[i]);
        for (int j = 0; j < len && index < 16; j++) {
            input[index] = (uint8_t)frases[i][j];
            index++;
        }
    }
    
    // Preenche o restante do array com zeros se necessário
    while (index < 16) {
        input[index] = 0;
        index++;
    }
}

void convertHexToString(uint8_t input[16], char output[17]) {
    int index = 0;
    
    // Converte cada byte para caractere
    for (int i = 0; i < 16; i++) {
        // Para quando encontra um byte zero (fim da string)
        if (input[i] == 0) {
            break;
        }
        output[index] = (char)input[i];
        index++;
    }
    
    // Adiciona terminador de string
    output[index] = '\0';
}
