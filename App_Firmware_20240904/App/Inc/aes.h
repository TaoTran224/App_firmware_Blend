
#ifndef AES_H_
#define AES_H_

//#include "stdint.h"

//
// 使用AES128 CBC模式进行加密, 采用ZeroPadding.
// 每16字节为一个加密块, 如果不足16个字节则用0补足.
// output缓冲区必须能够容纳补足字节后的大小
//
// unsigned char encrypted_data[...];
// unsigned char data[...];
// unsigned int data_len = 100;
//
// unsigned int encrypted_data_len = aes_encrypt(encrypted_data, data, data_len, 0, 0);
// aes_decrypt(data, encrypted_data, encrypted_data_len, 0, 0);
//

/**
 * [aes_encrypt AES128加密]
 * @param output [输出密文缓冲区]
 * @param input  [输入明文缓冲区]
 * @param length [输入明文的字节数]
 * @param key    [密钥]
 * @param iv     [初始向量]
 */
unsigned int aes_encrypt(unsigned char *output, const unsigned char *input, unsigned int length, const unsigned char *key, const unsigned char *iv);

/**
 * [aes_decrypt AES128解密]
 * @param output [输出明文缓冲区]
 * @param input  [输入密文缓冲区]
 * @param length [输入密文的字节数]
 * @param key    [密钥]
 * @param iv     [初始向量]
 */
void aes_decrypt(unsigned char *output, const unsigned char *input, unsigned int length, const unsigned char *key, const unsigned char *iv);


#endif // AES_H_
