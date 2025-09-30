 #if 0
        uint8_t output[16] = {0};

        // XOR e criar state
        XorBlock(key, input, state);
        printf("\nMSBs extraídos do FirstBit:\n");
        FirstBit(&state);

        // Cifrar
        AES_ECB_encrypt(&ctx, input, output);

        // Imprimir MSBs do output cifrado
        printf("\nMSBs extraídos do SubBytes:\n"); 
        for (int i = 0; i < 16; i++) { 
            printf("%d ", output[i]); 
        }
        printf("\n");
    #else
        uint8_t MSB[16][10] = {0};

        // Inicializa contexto AES
       // AES_init_ctx(&ctx, key);

        // Criptografa e preenche MSB[16][10]
        AES_ECB_encrypt(&ctx, input, MSB);

        printf("\n\n");
            #if 0
            // Exibe MSBs por round
            for (int round = 0; round < 9; round++) 
            {
                printf("Round %d MSBs:  ", round + 1);
                for (int i = 0; i < 16; i++) {
                    printf("%d ", MSB[i][round]);
                }
                printf("\n");
            }
                printf("Round 10 MSBs: ");
            for (int i = 0; i < 16; i++) {
                    printf("%d ", MSB[i][9]);
                }
            printf("\n\n");
            #else
            PrintMSBStyled(MSB);
            #endif

    #endif


    
void PrintMSBStyled(uint8_t MSB[16][10])
{
    for (int round = 0; round < 10; round++)
    {
        printf("Round %d MSBs:\n", round + 1);
        for (int i = 0; i < 16; i++)
        {
            printf("%dxxx-xxxx  ", MSB[i][round]);  // imprime o MSB no estilo desejado

            // quebra de linha a cada 4 bytes para formato 4x4
            if ((i + 1) % 4 == 0)
                printf("\n");
        }
        printf("\n"); // linha extra entre rounds
    }
}


uint8_t input[16]  = { 0x6b,0xc1,0xbe,0xe2, 0x2e,0x40,0x9f,0x96,
                            0xe9,0x3d,0x7e,0x11, 0x73,0x93,0x17,0x2a };