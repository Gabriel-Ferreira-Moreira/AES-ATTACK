// Esse arquivo contém testes unitários para o AES usando Google Test framework.
// Ele verifica se o processo de encriptação e decriptação restaura corretamente o texto original.

#include <gtest/gtest.h>
#include <cstdint>
#include <cstring>
#include <iostream>
#include <cstdlib>   // system()
#include "aes.h" 

int main(int argc, char **argv) {
    // Limpa o terminal antes de rodar os testes
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif

    ::testing::InitGoogleTest(&argc, argv);
    int result = RUN_ALL_TESTS();

    if (result == 0) {
        std::cout << "\nAES Passou: texto foi restaurado!\n";
    }

    return result;
}

class AESTest : public ::testing::Test {
protected:
    AES_ctx ctx;
    uint8_t key[16] = {
        0x2b,0x7e,0x15,0x16,
        0x28,0xae,0xd2,0xa6,
        0xab,0xf7,0x15,0x88,
        0x09,0xcf,0x4f,0x3c
    };

    void SetUp() override { // Executado antes de cada teste
        AES_init_ctx(&ctx, key);
    }
};

// Testa o processor de encriptaçao e decriptaçao do AES 
TEST_F(AESTest, EncryptDecrypt) {
    uint8_t input[16] = {
        0x32,0x43,0xf6,0xa8,
        0x88,0x5a,0x30,0x8d,
        0x31,0x31,0x98,0xa2,
        0xe0,0x37,0x07,0x34
    };

    uint8_t save[16];
    memcpy(save, input, 16); // Salva o input original

    int count = 0;
    AES_ECB_encrypt(&ctx, input, &count);
    AES_ECB_decrypt(&ctx, input);

    EXPECT_EQ(memcmp(save, input, 16), 0) << "AES falhou: texto não foi restaurado.";
}


