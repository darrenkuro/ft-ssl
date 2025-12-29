// #include "libft.h"
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LEFTROTATE(x, c) (((x) << (c)) | ((x) >> (32 - (c))))

/* Per-round shift amounts */
static const uint32_t s[64] = {7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22,
                               5, 9,  14, 20, 5, 9,  14, 20, 5, 9,  14, 20, 5, 9,  14, 20,
                               4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23,
                               6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21};

/*
Table of constants K[i] = floor(2^32 * abs(sin(i + 1)))
    K[i] = (uint32_t) (fabs(sin(i + 1)) * (double)(1ULL << 32));
*/
static const uint32_t K[64] = {
    0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee, 0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501,
    0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be, 0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821,
    0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa, 0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8,
    0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed, 0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a,
    0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c, 0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70,
    0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05, 0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665,
    0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039, 0x655b59c3, 0x8f0ccc92, 0xffeff47d, 0x85845dd1,
    0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1, 0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391,
};

void md5(const uint8_t *msg, size_t len, uint8_t digest[16]) {
    uint32_t A = 0x67452301; // 01 23 45 67
    uint32_t B = 0xefcdab89; // 89 ab cd ef
    uint32_t C = 0x98badcfe; // fe dc ba 98
    uint32_t D = 0x10325476; // 76 54 32 10

    /* Padding */
    size_t new_len = len + 1;
    while (new_len % 64 != 56)
        new_len++;

    uint8_t *buf = calloc(new_len + 8, 1);
    memcpy(buf, msg, len);
    buf[len] = 0x80;

    uint64_t bits_len = (uint64_t)len * 8;
    memcpy(buf + new_len, &bits_len, 8); /* little-endian */

    /* Process blocks */
    for (size_t offset = 0; offset < new_len; offset += 64) {
        uint32_t *M = (uint32_t *)(buf + offset);

        uint32_t a = A;
        uint32_t b = B;
        uint32_t c = C;
        uint32_t d = D;

        for (uint32_t i = 0; i < 64; i++) {
            uint32_t f, g;

            if (i < 16) {
                f = (b & c) | (~b & d);
                g = i;
            } else if (i < 32) {
                f = (d & b) | (~d & c);
                g = (5 * i + 1) % 16;
            } else if (i < 48) {
                f = b ^ c ^ d;
                g = (3 * i + 5) % 16;
            } else {
                f = c ^ (b | ~d);
                g = (7 * i) % 16;
            }

            uint32_t temp = d;
            d = c;
            c = b;
            b = b + LEFTROTATE(a + f + K[i] + M[g], s[i]);
            a = temp;
        }

        A += a;
        B += b;
        C += c;
        D += d;
    }

    free(buf);

    memcpy(digest, &A, 4);
    memcpy(digest + 4, &B, 4);
    memcpy(digest + 8, &C, 4);
    memcpy(digest + 12, &D, 4);
}

/* ////
int main(int argc, char **argv) {
    uint8_t digest[16];

    if (argc != 2) {
        fprintf(stderr, "Usage: %s <string>\n", argv[0]);
        return 1;
    }

    md5((const uint8_t *)argv[1], strlen(argv[1]), digest);

    for (int i = 0; i < 16; i++)
        printf("%02x", digest[i]);
    printf("\n");

    return 0;
}
*/ ////
