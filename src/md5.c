/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   md5.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlu <dlu@student.42berlin.de>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/29 23:24:02 by dlu               #+#    #+#             */
/*   Updated: 2025/12/30 05:16:42 by dlu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// #include "libft.h"
#include "context.h"
#include "flags.h"
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct s_md5_ctx {
    uint32_t h[4];      // Internal hash states, A B C D
    uint64_t bitlen;    // Bits processed so far (need at the end)
    uint8_t buffer[64]; // Partial input block
    size_t buffer_len;  // Current buffer length
} t_md5_ctx;

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

static void md5_process_block(t_md5_ctx *ctx, const uint8_t block[64]) {
    uint32_t a = ctx->h[0];
    uint32_t b = ctx->h[1];
    uint32_t c = ctx->h[2];
    uint32_t d = ctx->h[3];

    const uint32_t *M = (const uint32_t *)block;

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

    ctx->h[0] += a;
    ctx->h[1] += b;
    ctx->h[2] += c;
    ctx->h[3] += d;
}

static void md5_init(void *vctx) {
    t_md5_ctx *ctx = vctx;

    ctx->h[0] = 0x67452301; // 01 23 45 67
    ctx->h[1] = 0xefcdab89; // 89 ab cd ef
    ctx->h[2] = 0x98badcfe; // fe dc ba 98
    ctx->h[3] = 0x10325476; // 76 54 32 10

    ctx->bitlen = 0;
    ctx->buffer_len = 0;
}

static void md5_update(void *vctx, const uint8_t *data, size_t len) {
    t_md5_ctx *ctx = vctx;

    ctx->bitlen += (uint64_t)len * 8;

    while (len > 0) {
        size_t to_copy = 64 - ctx->buffer_len;
        if (to_copy > len)
            to_copy = len;

        memcpy(ctx->buffer + ctx->buffer_len, data, to_copy);
        ctx->buffer_len += to_copy;
        data += to_copy;
        len -= to_copy;

        if (ctx->buffer_len == 64) {
            md5_process_block(ctx, ctx->buffer);
            ctx->buffer_len = 0;
        }
    }
}

static void md5_final(void *vctx, uint8_t *out) {
    t_md5_ctx *ctx = vctx;

    /* append 0x80 */
    ctx->buffer[ctx->buffer_len++] = 0x80;

    /* pad with zeros until 56 bytes */
    if (ctx->buffer_len > 56) {
        while (ctx->buffer_len < 64)
            ctx->buffer[ctx->buffer_len++] = 0x00;
        md5_process_block(ctx, ctx->buffer);
        ctx->buffer_len = 0;
    }

    while (ctx->buffer_len < 56)
        ctx->buffer[ctx->buffer_len++] = 0x00;

    /* append original length in bits (little endian) */
    memcpy(ctx->buffer + 56, &ctx->bitlen, 8);
    md5_process_block(ctx, ctx->buffer);

    /* output digest (little endian) */
    memcpy(out, &ctx->h[0], 4);
    memcpy(out + 4, &ctx->h[1], 4);
    memcpy(out + 8, &ctx->h[2], 4);
    memcpy(out + 12, &ctx->h[3], 4);
}

static void md5_print(const uint8_t *digest, const t_global_ctx *gctx, const t_job *job) {
    bool quiet = gctx->flags & FLAG_Q;
    bool reverse = gctx->flags & FLAG_R;

    if (!reverse && !quiet) {
        if (job->type == I_STRING)
            printf("MD5 (\"%s\") = ", job->input);
        if (job->type == I_FILE)
            printf("MD5 (%s) = ", job->input);
        if (job->type == I_STDIN && !(gctx->flags & FLAG_P))
            printf("(stdin) = ");
    }

    for (int i = 0; i < 16; i++)
        printf("%02x", digest[i]);

    if (reverse && !quiet) {
        if (job->type == I_STRING)
            printf(" \"%s\"", job->input);
        if (job->type == I_FILE)
            printf(" %s", job->input);
    }

    printf("\n");
}

const t_algo g_md5_algo = {.name = "md5",
                           .ctx_size = sizeof(t_md5_ctx),
                           .init = md5_init,
                           .update = md5_update,
                           .final = md5_final,
                           .digest_size = 16,
                           .print = md5_print};
