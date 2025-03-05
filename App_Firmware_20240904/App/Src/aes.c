
#include "TI_aes_128.h"
#include "aes.h"
#include "string.h"


#define BLOCK_SIZE     (unsigned char) 16


const static unsigned char aes_key_default[16] =
        {0xE2, 0x83, 0x01, 0x62, 0x75, 0x8C, 0x69, 0x71, 0xF3, 0xFE, 0x98, 0x77, 0x6E, 0x4C, 0x94, 0xD5};

const static unsigned char aes_iv_default[16] =
        {0x6F, 0x12, 0x6A, 0x10, 0x12, 0x48, 0x74, 0xF7, 0xC6, 0xF5, 0xE9, 0x03, 0x29, 0x59, 0x35, 0x18};


static void xor_iv(unsigned char *block, const unsigned char *iv)
{
    unsigned char i;
    for (i = (unsigned char)0; i < BLOCK_SIZE; i++) {
        block[i] ^= iv[i];
    }
}

unsigned int aes_encrypt(unsigned char *output, const unsigned char *input, unsigned int length, const unsigned char *key, const unsigned char *iv)
{
    unsigned char akey[BLOCK_SIZE];
    unsigned char remainders = (unsigned char)(length % BLOCK_SIZE);     
    unsigned int i = 0;

    if (key == 0) {
        key = aes_key_default;
    }

    if (iv == 0) {
        iv = aes_iv_default;
    }

    for (i = (unsigned int)0; i < (unsigned int)(length / BLOCK_SIZE); i++) {
        memcpy(output, input, BLOCK_SIZE);
        xor_iv(output, iv);
        memcpy(akey, key, BLOCK_SIZE);
        aes_enc_dec(output, akey, 0);
        iv = output;
        input += BLOCK_SIZE;
        output += BLOCK_SIZE;
    }

    if (remainders > (unsigned int)0) {
        unsigned char padding_len = (unsigned char)(BLOCK_SIZE - remainders);
        memcpy(output, input, remainders);
        memset(&output[remainders], 0, padding_len);
        xor_iv(output, iv);
        memcpy(akey, key, BLOCK_SIZE);
        aes_enc_dec(output, akey, 0);

        length += padding_len;
    }

    return length;
}

void aes_decrypt(unsigned char *output, const unsigned char *input, unsigned int length, const unsigned char *key, const unsigned char *iv)
{
    unsigned char akey[BLOCK_SIZE];  
    unsigned int i = 0;

    if (key == 0) {
        memcpy(akey, aes_key_default, BLOCK_SIZE);
    } else {
        memcpy(akey, key, BLOCK_SIZE);
    }

    if (iv == 0) {
        iv = aes_iv_default;
    }

    for ( i = 0; i < length / BLOCK_SIZE; i++) {
        memcpy(output, input, BLOCK_SIZE);
        aes_enc_dec(output, akey, 1);
        xor_iv(output, iv);
        iv = input;
        input += BLOCK_SIZE;
        output += BLOCK_SIZE;
    }
}
