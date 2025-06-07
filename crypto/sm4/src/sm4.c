#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>
#include "../inc/sm4.h"
#define maxm 5 * 1024 * 1024 + 10
// uint32_t x循环左移n位
#define ROF32(x, n) (((x) << (n)) | ((x) >> (32 - (n))))

// uint8_t y[4] -> uint32_t x
#define LOAD32H(x, y)                                                                   \
    do                                                                                  \
    {                                                                                   \
        (x) = ((uint32_t)((y)[0] & 0x0ff) << 24) | ((uint32_t)((y)[1] & 0x0ff) << 16) | \
              ((uint32_t)((y)[2] & 0x0ff) << 8) | ((uint32_t)((y)[3] & 0x0ff));         \
    } while (0)

// uint32_t x -> uint8_t y[4]
#define STORE32H(x, y)                           \
    do                                           \
    {                                            \
        (y)[0] = (uint8_t)(((x) >> 24) & 0x0ff); \
        (y)[1] = (uint8_t)(((x) >> 16) & 0x0ff); \
        (y)[2] = (uint8_t)(((x) >> 8) & 0x0ff);  \
        (y)[3] = (uint8_t)((x) & 0x0ff);         \
    } while (0)

#define s_box_op(in, out)                             \
    {                                                 \
        (out) = (s_box[((in) >> 24) & 0x0ff]) << 24;  \
        (out) |= (s_box[((in) >> 16) & 0x0ff]) << 16; \
        (out) |= (s_box[((in) >> 8) & 0x0ff]) << 8;   \
        (out) |= (s_box[(in) & 0x0ff]);               \
    }

typedef struct
{
    uint32_t rk[32];      // 轮密钥
    uint8_t buf[16];      // 待加密数据
    uint8_t iv[16];       // CBC IV向量
    uint32_t cur_buf_len; // 当前待压缩消息长度（字节）
    uint32_t total_len;   // 已加密数据长度（字节）
    uint32_t state;       // 0比特标识是否为加密，1比特标识是否为PKCS7填充，2比特标识是否为CBC加密
} sm4_context;

static uint8_t s_box[256] = {
    0xd6, 0x90, 0xe9, 0xfe, 0xcc, 0xe1, 0x3d, 0xb7, 0x16, 0xb6, 0x14, 0xc2, 0x28, 0xfb, 0x2c, 0x05,
    0x2b, 0x67, 0x9a, 0x76, 0x2a, 0xbe, 0x04, 0xc3, 0xaa, 0x44, 0x13, 0x26, 0x49, 0x86, 0x06, 0x99,
    0x9c, 0x42, 0x50, 0xf4, 0x91, 0xef, 0x98, 0x7a, 0x33, 0x54, 0x0b, 0x43, 0xed, 0xcf, 0xac, 0x62,
    0xe4, 0xb3, 0x1c, 0xa9, 0xc9, 0x08, 0xe8, 0x95, 0x80, 0xdf, 0x94, 0xfa, 0x75, 0x8f, 0x3f, 0xa6,
    0x47, 0x07, 0xa7, 0xfc, 0xf3, 0x73, 0x17, 0xba, 0x83, 0x59, 0x3c, 0x19, 0xe6, 0x85, 0x4f, 0xa8,
    0x68, 0x6b, 0x81, 0xb2, 0x71, 0x64, 0xda, 0x8b, 0xf8, 0xeb, 0x0f, 0x4b, 0x70, 0x56, 0x9d, 0x35,
    0x1e, 0x24, 0x0e, 0x5e, 0x63, 0x58, 0xd1, 0xa2, 0x25, 0x22, 0x7c, 0x3b, 0x01, 0x21, 0x78, 0x87,
    0xd4, 0x00, 0x46, 0x57, 0x9f, 0xd3, 0x27, 0x52, 0x4c, 0x36, 0x02, 0xe7, 0xa0, 0xc4, 0xc8, 0x9e,
    0xea, 0xbf, 0x8a, 0xd2, 0x40, 0xc7, 0x38, 0xb5, 0xa3, 0xf7, 0xf2, 0xce, 0xf9, 0x61, 0x15, 0xa1,
    0xe0, 0xae, 0x5d, 0xa4, 0x9b, 0x34, 0x1a, 0x55, 0xad, 0x93, 0x32, 0x30, 0xf5, 0x8c, 0xb1, 0xe3,
    0x1d, 0xf6, 0xe2, 0x2e, 0x82, 0x66, 0xca, 0x60, 0xc0, 0x29, 0x23, 0xab, 0x0d, 0x53, 0x4e, 0x6f,
    0xd5, 0xdb, 0x37, 0x45, 0xde, 0xfd, 0x8e, 0x2f, 0x03, 0xff, 0x6a, 0x72, 0x6d, 0x6c, 0x5b, 0x51,
    0x8d, 0x1b, 0xaf, 0x92, 0xbb, 0xdd, 0xbc, 0x7f, 0x11, 0xd9, 0x5c, 0x41, 0x1f, 0x10, 0x5a, 0xd8,
    0x0a, 0xc1, 0x31, 0x88, 0xa5, 0xcd, 0x7b, 0xbd, 0x2d, 0x74, 0xd0, 0x12, 0xb8, 0xe5, 0xb4, 0xb0,
    0x89, 0x69, 0x97, 0x4a, 0x0c, 0x96, 0x77, 0x7e, 0x65, 0xb9, 0xf1, 0x09, 0xc5, 0x6e, 0xc6, 0x84,
    0x18, 0xf0, 0x7d, 0xec, 0x3a, 0xdc, 0x4d, 0x20, 0x79, 0xee, 0x5f, 0x3e, 0xd7, 0xcb, 0x39, 0x48};

static uint32_t ck[32] = {
    0x00070e15, 0x1c232a31, 0x383f464d, 0x545b6269,
    0x70777e85, 0x8c939aa1, 0xa8afb6bd, 0xc4cbd2d9,
    0xe0e7eef5, 0xfc030a11, 0x181f262d, 0x343b4249,
    0x50575e65, 0x6c737a81, 0x888f969d, 0xa4abb2b9,
    0xc0c7ced5, 0xdce3eaf1, 0xf8ff060d, 0x141b2229,
    0x30373e45, 0x4c535a61, 0x686f767d, 0x848b9299,
    0xa0a7aeb5, 0xbcc3cad1, 0xd8dfe6ed, 0xf4fb0209,
    0x10171e25, 0x2c333a41, 0x484f565d, 0x646b7279};

static uint32_t fk[4] = {
    0xA3B1BAC6, 0x56AA3350, 0x677D9197, 0xB27022DC};

int sm4_make_enc_subkeys(const unsigned char key[SM4_KEY_SIZE], uint32_t encSubKeys[SM4_ROUNDS])
{
    uint32_t K[36], tmp1, tmp2;
    LOAD32H(K[0], key);
    LOAD32H(K[1], key + 4);
    LOAD32H(K[2], key + 8);
    LOAD32H(K[3], key + 12);

    K[0] ^= fk[0];
    K[1] ^= fk[1];
    K[2] ^= fk[2];
    K[3] ^= fk[3];

    for (int i = 0; i < 32; i++)
    {
        tmp1 = K[i + 1] ^ K[i + 2] ^ K[i + 3] ^ ck[i];

        // 非线性变换(s_box)
        s_box_op(tmp1, tmp2);

        // 线性变换(左移)
        encSubKeys[i] = K[i + 4] = K[i] ^ (tmp2 ^ ROF32(tmp2, 13) ^ ROF32(tmp2, 23));
    }
    return 0;
}

int sm4_make_dec_subkeys(const unsigned char key[SM4_KEY_SIZE], uint32_t decSubKeys[SM4_ROUNDS])
{
    uint32_t K[36], tmp1, tmp2;
    LOAD32H(K[0], key);
    LOAD32H(K[1], key + 4);
    LOAD32H(K[2], key + 8);
    LOAD32H(K[3], key + 12);

    K[0] ^= fk[0];
    K[1] ^= fk[1];
    K[2] ^= fk[2];
    K[3] ^= fk[3];

    for (int i = 0; i < 32; i++)
    {
        tmp1 = K[i + 1] ^ K[i + 2] ^ K[i + 3] ^ ck[i];

        // 非线性变换(s_box)
        s_box_op(tmp1, tmp2);

        // 线性变换(左移)
        decSubKeys[31 - i] = K[i + 4] = K[i] ^ (tmp2 ^ ROF32(tmp2, 13) ^ ROF32(tmp2, 23));
    }
    return 0;
}

void sm4_encrypt_block(const unsigned char *input, const uint32_t *encSubKeys, unsigned char *output)
{
    uint32_t X[36];
    uint32_t tmp1, tmp2;
    LOAD32H(X[0], input);
    LOAD32H(X[1], input + 4);
    LOAD32H(X[2], input + 8);
    LOAD32H(X[3], input + 12);

    for (int i = 0; i < 32; i++)
    {
        tmp1 = X[i + 1] ^ X[i + 2] ^ X[i + 3] ^ encSubKeys[i];

        // 非线性变换(s_box)
        s_box_op(tmp1, tmp2);

        // 线性变换(左移)
        uint32_t tt = (tmp2 ^ ROF32(tmp2, 2) ^ ROF32(tmp2, 10) ^ ROF32(tmp2, 18) ^ ROF32(tmp2, 24));

        X[i + 4] = X[i] ^ tt;
    }

    STORE32H(X[35], output);
    STORE32H(X[34], output + 4);
    STORE32H(X[33], output + 8);
    STORE32H(X[32], output + 12);
}

void sm4_decrypt_block(const unsigned char *input, const uint32_t *decSubKeys, unsigned char *output)
{
    uint32_t X[36];
    uint32_t tmp1, tmp2;
    LOAD32H(X[0], input);
    LOAD32H(X[1], input + 4);
    LOAD32H(X[2], input + 8);
    LOAD32H(X[3], input + 12);

    for (int i = 0; i < 32; i++)
    {
        tmp1 = X[i + 1] ^ X[i + 2] ^ X[i + 3] ^ decSubKeys[i];

        // 非线性变换(s_box)
        s_box_op(tmp1, tmp2);

        // 线性变换(左移)
        uint32_t tt = (tmp2 ^ ROF32(tmp2, 2) ^ ROF32(tmp2, 10) ^ ROF32(tmp2, 18) ^ ROF32(tmp2, 24));

        X[i + 4] = X[i] ^ tt;
    }

    STORE32H(X[35], output);
    STORE32H(X[34], output + 4);
    STORE32H(X[33], output + 8);
    STORE32H(X[32], output + 12);
}

void xor_blocks(const unsigned char *in1, const unsigned char *in2, unsigned char *out)
{
    for (int i = 0; i < SM4_BLOCK_SIZE; i++)
    {
        out[i] = in1[i] ^ in2[i];
    }
}

void sm4_cbc_encrypt(const unsigned char *plaintext, unsigned char *ciphertext, int length, const uint32_t key[16], const unsigned char iv[16])
{
    unsigned char block[SM4_BLOCK_SIZE];
    unsigned char iv_buf[SM4_BLOCK_SIZE];
    memcpy(iv_buf, iv, SM4_BLOCK_SIZE);

    for (int i = 0; i < length; i += SM4_BLOCK_SIZE)
    {
        xor_blocks(plaintext + i, iv_buf, block);
        sm4_encrypt_block(block, key, ciphertext + i);
        memcpy(iv_buf, ciphertext + i, SM4_BLOCK_SIZE);
    }
}

void sm4_cbc_decrypt(const unsigned char *ciphertext, unsigned char *plaintext, int length, const uint32_t key[16], const unsigned char iv[16])
{
    unsigned char block[SM4_BLOCK_SIZE];
    unsigned char iv_buf[SM4_BLOCK_SIZE];
    memcpy(iv_buf, iv, SM4_BLOCK_SIZE);

    for (int i = 0; i < length; i += SM4_BLOCK_SIZE)
    {
        sm4_decrypt_block(ciphertext + i, key, block);
        xor_blocks(block, iv_buf, plaintext + i);
        memcpy(iv_buf, ciphertext + i, SM4_BLOCK_SIZE);
    }
}

unsigned char hex_to_byte(char high, char low)
{
    unsigned char high_val = (high >= '0' && high <= '9') ? (high - '0') : (high >= 'a' && high <= 'f') ? (high - 'a' + 10)
                                                                       : (high >= 'A' && high <= 'F')   ? (high - 'A' + 10)
                                                                                                        : 0;

    unsigned char low_val = (low >= '0' && low <= '9') ? (low - '0') : (low >= 'a' && low <= 'f') ? (low - 'a' + 10)
                                                                   : (low >= 'A' && low <= 'F')   ? (low - 'A' + 10)
                                                                                                  : 0;

    return (high_val << 4) | low_val;
}

int main()
{
    printf("zhunbeiduqu");
    // 测试数据
    const uint32_t key[16] = {0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF,
                              0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF}; // 128位密钥
    const unsigned char iv[3][16] = {
        {0xf4, 0xdc, 0xd9, 0xb7, 0xcb, 0xf7, 0xe8, 0xa5, 0xf4, 0xe6, 0x23, 0x1d, 0x4c, 0xdf, 0x75, 0xb3},
        {0x25, 0xbd, 0xdd, 0xba, 0x9a, 0x02, 0x63, 0x65, 0x20, 0x1c, 0x50, 0x2c, 0x5e, 0xe7, 0x9e, 0x71},
        {0x3f, 0x74, 0xae, 0x64, 0x45, 0x09, 0x39, 0x24, 0x20, 0x68, 0x89, 0x27, 0x7f, 0x54, 0xe9, 0xdb}}; // 128位IV

    FILE *file = fopen("E:\\verity\\第三次实验-2022302181271-张皓宇\\src\\64B.txt", "r"); // 打开txt文件
    if (file == NULL)
    {
        perror("Failed to open file");
        return 1;
    }
    unsigned char plaintext[maxm];
    size_t index = 0;
    char ch;
    // 读取文件内容，并将每两个字符作为一个16进制字节
    while ((ch = fgetc(file)) != EOF)
    {
        printf("正在读取文件");
        // 跳过空白字符
        if (isspace(ch))
        {
            continue;
        }

        // 存储第一个16进制字符
        char high = ch;

        // 读取下一个字符，合并成一个字节
        ch = fgetc(file);
        if (ch == EOF)
            break;

        char low = ch;

        // 转换16进制字符为字节并存储
        plaintext[index++] = hex_to_byte(high, low);
    }

    fclose(file);

    int length = strlen((char *)plaintext);
    unsigned char ciphertext[SM4_BLOCK_SIZE * ((length / SM4_BLOCK_SIZE) + 1)];
    unsigned char decryptedtext[sizeof(ciphertext)];

    // 打印原始明文
    printf("Original plaintext: ");
    for (int i = 0; i < length; i++)
    {
        printf("%02X", plaintext[i]);
    }
    printf("\n");

    // 执行加密
    sm4_cbc_encrypt(plaintext, ciphertext, length, key, iv[1]);
    printf("Encrypted ciphertext: ");
    for (int i = 0; i < length; i++)
    {
        printf("%02X", ciphertext[i]);
    }
    printf("\n");

    // 执行解密
    sm4_cbc_decrypt(ciphertext, decryptedtext, length, key, iv[1]);

    printf("Decrypted plaintext: ");
    for (int i = 0; i < length; i++)
    {
        printf("%02X", decryptedtext[i]);
    }
    printf("\n");

    // 检查解密后的明文是否与原始明文一致
    assert(memcmp(plaintext, decryptedtext, length) == 0);

    printf("Decryption successful! The plaintext is correct.\n");
    return 0;
}