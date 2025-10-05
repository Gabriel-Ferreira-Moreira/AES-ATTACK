#ifndef AES_H_
#define AES_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#define RDN 1 // 0 para analisar 1 round/1 para analisar todos os rounds
#define BR 0  // 0 para oráculo 2 e 1 para oráculo 1  
#define ECB 1
#define AES128 1

#define AES_BLOCKLEN 16 // 16 bytes = 128 bits (tamanho do bloco AES)
#define AES_KEYLEN 16   // chave de 128 bits (16 bytes)
#define AES_keyExpSize 176 // tamanho da chave expandida

struct AES_ctx
{
  uint8_t RoundKey[AES_keyExpSize]; // Guarda o conjunto de chaves expandidas (round keys)
};

void AES_init_ctx(struct AES_ctx* ctx, const uint8_t* key);
void AES_ECB_encrypt(const struct AES_ctx* ctx, uint8_t* buf, int *O);
void AES_ECB_decrypt(const struct AES_ctx* ctx, uint8_t* buf);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif // AES_H_
