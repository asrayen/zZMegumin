#include <stdio.h>
#include <string.h>

int IP[64] = {58, 50, 42, 34, 26, 18, 10, 2,
              60, 52, 44, 36, 28, 20, 12, 4,
              62, 54, 46, 38, 30, 22, 14, 6,
              64, 56, 48, 40, 32, 24, 16, 8,
              47, 49, 41, 33, 25, 17, 9, 1,
              59, 51, 43, 35, 27, 19, 11, 3,
              61, 53, 45, 37, 29, 21, 13, 5,
              63, 55, 47, 39, 31, 23, 15, 7};

int P1[56] = {57, 49, 41, 33, 25, 17, 9,
              1, 58, 50, 42, 34, 26, 18,
              10, 2, 59, 51, 43, 35, 27,
              19, 11, 3, 60, 52, 44, 36,
              63, 55, 47, 39, 31, 23, 15,
              7, 62, 54, 46, 38, 30, 22,
              14, 6, 61, 53, 45, 37, 29,
              21, 13, 5, 28, 20, 12, 4};

int P2[48] = {14, 17, 11, 24, 1, 5,
              3, 28, 15, 6, 21, 10,
              23, 19, 12, 4, 26, 8,
              16, 7, 27, 20, 13, 2,
              41, 52, 31, 37, 47, 55,
              30, 40, 51, 45, 33, 48,
              44, 49, 39, 56, 34, 53,
              46, 42, 50, 36, 29, 32};
int E[48] = {32, 1, 2, 3, 4, 5,
             4, 5, 6, 7, 8, 9,
             8, 9, 10, 11, 12, 13,
             12, 13, 14, 15, 16, 17,
             16, 17, 18, 19, 20, 21,
             20, 21, 22, 23, 24, 25,
             24, 25, 26, 27, 28, 29,
             28, 29, 30, 31, 32, 1};
int S_BOX[8][64] = {{14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7,
                     0, 15, 7, 4, 14, 2, 13, 1, 10, 6, 12, 11, 9, 5, 3, 8,
                     4, 1, 14, 8, 13, 6, 2, 11, 15, 12, 9, 7, 3, 10, 5, 0,
                     15, 12, 8, 2, 4, 9, 1, 7, 5, 11, 3, 14, 10, 0, 6, 13},
                    {15, 1, 8, 14, 6, 11, 3, 4, 9, 7, 2, 13, 12, 0, 5, 10,
                     3, 13, 4, 7, 15, 2, 8, 14, 12, 0, 1, 10, 6, 9, 11, 5,
                     0, 14, 7, 11, 10, 4, 13, 1, 5, 8, 12, 6, 9, 3, 2, 15,
                     12, 8, 10, 1, 3, 15, 4, 2, 11, 6, 7, 12, 0, 5, 14, 9},
                    {10, 0, 9, 14, 6, 3, 15, 5, 1, 13, 12, 7, 11, 4, 2, 8,
                     13, 7, 0, 9, 3, 4, 6, 10, 2, 8, 5, 14, 12, 11, 15, 1,
                     13, 6, 4, 9, 8, 15, 3, 0, 11, 1, 2, 12, 5, 10, 14, 7,
                     1, 10, 13, 0, 6, 9, 8, 7, 4, 15, 14, 3, 11, 5, 2, 12},
                    {7, 13, 14, 3, 0, 6, 9, 10, 1, 2, 8, 5, 11, 12, 4, 15,
                     13, 8, 11, 5, 6, 15, 0, 3, 4, 7, 2, 12, 1, 10, 14, 9,
                     10, 6, 9, 0, 12, 11, 7, 13, 15, 1, 3, 14, 5, 2, 8, 4,
                     3, 15, 0, 6, 10, 1, 13, 8, 9, 4, 5, 11, 12, 7, 2, 14},
                    {2, 12, 4, 1, 7, 10, 11, 6, 8, 5, 3, 15, 13, 0, 14, 9,
                     14, 11, 2, 12, 4, 7, 13, 1, 5, 0, 15, 10, 3, 9, 8, 6,
                     4, 2, 1, 11, 10, 13, 7, 8, 15, 9, 12, 5, 6, 3, 0, 14,
                     11, 8, 12, 7, 1, 14, 2, 13, 6, 15, 0, 9, 10, 4, 5, 3},
                    {12, 1, 10, 15, 9, 2, 6, 8, 0, 13, 3, 4, 14, 7, 5, 11,
                     10, 15, 4, 2, 7, 12, 9, 5, 6, 1, 13, 14, 0, 11, 3, 8,
                     9, 14, 15, 5, 2, 8, 12, 3, 7, 0, 4, 10, 1, 13, 11, 6,
                     4, 3, 2, 12, 9, 5, 15, 10, 11, 14, 1, 7, 6, 0, 8, 13},
                    {4, 11, 2, 14, 15, 0, 8, 13, 3, 12, 9, 7, 5, 10, 6, 1,
                     13, 0, 11, 7, 4, 9, 1, 10, 14, 3, 5, 12, 2, 15, 8, 6,
                     1, 4, 11, 13, 12, 3, 7, 14, 10, 15, 6, 8, 0, 5, 9, 2,
                     6, 11, 13, 8, 1, 4, 10, 7, 9, 5, 0, 15, 14, 2, 3, 12},
                    {13, 2, 8, 4, 6, 15, 11, 1, 10, 9, 3, 14, 5, 0, 12, 7,
                     1, 15, 13, 8, 10, 3, 7, 4, 12, 5, 6, 11, 0, 14, 9, 2,
                     7, 11, 4, 1, 9, 12, 14, 2, 0, 6, 10, 13, 15, 3, 5, 8,
                     2, 1, 14, 7, 4, 10, 8, 13, 15, 12, 9, 0, 3, 5, 6, 11}};
int P[32] = {16, 7, 20, 21,
             29, 12, 28, 17,
             1, 15, 23, 26,
             5, 18, 31, 10,
             2, 8, 24, 14,
             32, 27, 3, 9,
             19, 13, 30, 6,
             22, 11, 4, 25};
int IP_INV[64] = {40, 8, 48, 16, 56, 24, 64, 32,
                  39, 7, 47, 15, 55, 23, 63, 31,
                  38, 6, 46, 14, 54, 22, 62, 30,
                  37, 5, 45, 13, 53, 21, 61, 29,
                  36, 4, 44, 12, 52, 20, 60, 28,
                  35, 3, 43, 11, 51, 19, 59, 27,
                  34, 2, 42, 10, 50, 18, 58, 26,
                  33, 1, 41, 9, 49, 17, 57, 25};
int Times[16] = {1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1};

// 通用置换函数
void permute(const unsigned char *input, unsigned char *output, const int *table, int n)
{
    memset(output, 0, (n + 7) / 8);
    for (int i = 0; i < n; i++)
    {
        int bit = table[i] - 1;
        int byte = bit / 8;
        int offset = bit % 8;
        if (input[byte] & (1 << (7 - offset)))
        {
            output[i / 8] |= (1 << (7 - (i % 8)));
        }
    }
}

// Feistel函数
void feistel(const unsigned char *right, const unsigned char *subKey, unsigned char *output)
{
    unsigned char expanded[6] = {0};
    unsigned char temp[6] = {0};
    unsigned char sboxOutput[4] = {0};

    // 扩展置换
    permute(right, expanded, E, 48);

    // 与子密钥异或
    for (int i = 0; i < 6; i++)
    {
        temp[i] = expanded[i] ^ subKey[i];
    }

    // S盒替换
    for (int i = 0; i < 8; i++)
    {
        int row = ((temp[i / 6] & 0x80) >> 6) | ((temp[i / 6] & 0x04) >> 2);
        int col = (temp[i / 6] & 0x78) >> 3;
        sboxOutput[i / 2] |= S_BOX[i][row * 16 + col] << ((i % 2) ? 0 : 4);
    }

    // P置换
    permute(sboxOutput, output, P, 32);
}

// 密钥左移函数
void rotate_left(unsigned char *half, int n, int shifts)
{
    unsigned int value = (half[0] << 24) | (half[1] << 16) | (half[2] << 8) | half[3];
    value = ((value << shifts) | (value >> (n - shifts))) & 0xFFFFFFF0; // 仅保留28位有效位
    half[0] = (value >> 24) & 0xFF;
    half[1] = (value >> 16) & 0xFF;
    half[2] = (value >> 8) & 0xFF;
    half[3] = value & 0xF0;
}

// 生成子密钥
int des_make_subkeys(const unsigned char key[8], unsigned char subKeys[16][6])
{
    unsigned char permutedKey[7] = {0};
    unsigned char c[4] = {0}, d[4] = {0};

    // PC1置换
    permute(key, permutedKey, P1, 56);

    // 拆分C和D
    memcpy(c, permutedKey, 4);
    c[3] &= 0xF0; // 保留高4位
    memcpy(d, permutedKey + 3, 4);
    d[0] &= 0x0F; // 保留低4位

    // 生成16轮子密钥
    for (int i = 0; i < 16; i++)
    {
        rotate_left(c, 28, Times[i]);
        rotate_left(d, 28, Times[i]);

        // 合并C和D
        unsigned char temp[7] = {0};
        memcpy(temp, c, 4);
        temp[3] |= (d[0] >> 4);
        memcpy(temp + 4, d + 1, 3);

        // PC2置换
        permute(temp, subKeys[i], P2, 48);
    }
    return 0;
}

// DES加密单个块
void des_encrypt_block(const unsigned char *input, unsigned char subKeys[16][6], unsigned char *output)
{
    unsigned char permutedBlock[8] = {0};
    unsigned char left[4] = {0}, right[4] = {0}, temp[4] = {0};

    // 初始置换
    permute(input, permutedBlock, IP, 64);
    memcpy(left, permutedBlock, 4);
    memcpy(right, permutedBlock + 4, 4);

    // 16轮Feistel网络
    for (int i = 0; i < 16; i++)
    {
        memcpy(temp, right, 4);
        feistel(right, subKeys[i], right);
        for (int j = 0; j < 4; j++)
        {
            right[j] ^= left[j];
        }
        memcpy(left, temp, 4);
    }

    // 左右交换并逆置换
    unsigned char preOutput[8] = {0};
    memcpy(preOutput, right, 4);
    memcpy(preOutput + 4, left, 4);
    permute(preOutput, output, IP_INV, 64);
}

// DES解密单个块
void des_decrypt_block(const unsigned char *input, unsigned char subKeys[16][6], unsigned char *output)
{
    unsigned char permutedBlock[8] = {0};
    unsigned char left[4] = {0}, right[4] = {0}, temp[4] = {0};

    // 初始置换
    permute(input, permutedBlock, IP, 64);
    memcpy(left, permutedBlock, 4);
    memcpy(right, permutedBlock + 4, 4);

    // 16轮Feistel网络（子密钥逆序）
    for (int i = 15; i >= 0; i--)
    {
        memcpy(temp, right, 4);
        feistel(right, subKeys[i], right);
        for (int j = 0; j < 4; j++)
        {
            right[j] ^= left[j];
        }
        memcpy(left, temp, 4);
    }

    // 左右交换并逆置换
    unsigned char preOutput[8] = {0};
    memcpy(preOutput, right, 4);
    memcpy(preOutput + 4, left, 4);
    permute(preOutput, output, IP_INV, 64);
}