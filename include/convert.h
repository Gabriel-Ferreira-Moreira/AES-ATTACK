#ifndef CONVERT_H
#define CONVERT_H

#include <stdint.h>  
#include <stdlib.h> 
#include <stdio.h>
typedef uint8_t state_t[4][4];
#define STATE 1

void convertStringArrayToHex(char frases[][50], int numFrases, uint8_t input[16]);
void convertHexToString(uint8_t input[16], char output[17]);
void FirstBit(state_t* state);
void XorBlock(const uint8_t key[16], const uint8_t input[16], state_t state);
void vectorToState(uint8_t output[16], state_t state);

#endif // CONVERT_H
