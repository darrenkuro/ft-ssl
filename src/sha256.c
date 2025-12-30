/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sha256.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlu <dlu@student.42berlin.de>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/29 23:23:59 by dlu               #+#    #+#             */
/*   Updated: 2025/12/29 23:30:23 by dlu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "context.h"
#include <stdint.h>
#include <stdio.h>

#define ROTR32(x, n) (((x) >> (n)) | ((x) << (32 - (n))))
#define SHR(x, n) ((x) >> (n))

#define CH(x, y, z) (((x) & (y)) ^ (~(x) & (z)))
#define MAJ(x, y, z) (((x) & (y)) ^ ((x) & (z)) ^ ((y) & (z)))

#define BSIG0(x) (ROTR32((x), 2) ^ ROTR32((x), 13) ^ ROTR32((x), 22))
#define BSIG1(x) (ROTR32((x), 6) ^ ROTR32((x), 11) ^ ROTR32((x), 25))
#define SSIG0(x) (ROTR32((x), 7) ^ ROTR32((x), 18) ^ SHR((x), 3))
#define SSIG1(x) (ROTR32((x), 17) ^ ROTR32((x), 19) ^ SHR((x), 10))

typedef struct s_sha256_ctx {
    uint32_t h[8];      // Internal hash states, h0-h7
    uint64_t bitlen;    // Bits processed so far (need at the end)
    uint8_t buffer[64]; // Partial input block
    size_t buffer_len;  // Current buffer length
} t_sha256_ctx;

static const uint32_t K[64] = {
    0x428a2f98u, 0x71374491u, 0xb5c0fbcfu, 0xe9b5dba5u, 0x3956c25bu, 0x59f111f1u, 0x923f82a4u,
    0xab1c5ed5u, 0xd807aa98u, 0x12835b01u, 0x243185beu, 0x550c7dc3u, 0x72be5d74u, 0x80deb1feu,
    0x9bdc06a7u, 0xc19bf174u, 0xe49b69c1u, 0xefbe4786u, 0x0fc19dc6u, 0x240ca1ccu, 0x2de92c6fu,
    0x4a7484aau, 0x5cb0a9dcu, 0x76f988dau, 0x983e5152u, 0xa831c66du, 0xb00327c8u, 0xbf597fc7u,
    0xc6e00bf3u, 0xd5a79147u, 0x06ca6351u, 0x14292967u, 0x27b70a85u, 0x2e1b2138u, 0x4d2c6dfcu,
    0x53380d13u, 0x650a7354u, 0x766a0abbu, 0x81c2c92eu, 0x92722c85u, 0xa2bfe8a1u, 0xa81a664bu,
    0xc24b8b70u, 0xc76c51a3u, 0xd192e819u, 0xd6990624u, 0xf40e3585u, 0x106aa070u, 0x19a4c116u,
    0x1e376c08u, 0x2748774cu, 0x34b0bcb5u, 0x391c0cb3u, 0x4ed8aa4au, 0x5b9cca4fu, 0x682e6ff3u,
    0x748f82eeu, 0x78a5636fu, 0x84c87814u, 0x8cc70208u, 0x90befffau, 0xa4506cebu, 0xbef9a3f7u,
    0xc67178f2u,
};

/* ---- Big-endian helpers ---- */
static uint32_t read_be32(const uint8_t *p) {
    return ((uint32_t)p[0] << 24) | ((uint32_t)p[1] << 16) | ((uint32_t)p[2] << 8) |
           ((uint32_t)p[3]);
}

static void write_be32(uint8_t *out, uint32_t x) {
    out[0] = (uint8_t)(x >> 24);
    out[1] = (uint8_t)(x >> 16);
    out[2] = (uint8_t)(x >> 8);
    out[3] = (uint8_t)(x);
}

static void write_be64(uint8_t *out, uint64_t x) {
    out[0] = (uint8_t)(x >> 56);
    out[1] = (uint8_t)(x >> 48);
    out[2] = (uint8_t)(x >> 40);
    out[3] = (uint8_t)(x >> 32);
    out[4] = (uint8_t)(x >> 24);
    out[5] = (uint8_t)(x >> 16);
    out[6] = (uint8_t)(x >> 8);
    out[7] = (uint8_t)(x);
}

static void sha256_process_block(t_sha256_ctx *ctx, const uint8_t block[64]) {
    uint32_t W[64];

    for (int i = 0; i < 16; i++)
        W[i] = read_be32(block + (size_t)i * 4);

    for (int i = 16; i < 64; i++)
        W[i] = SSIG1(W[i - 2]) + W[i - 7] + SSIG0(W[i - 15]) + W[i - 16];

    uint32_t a = ctx->h[0];
    uint32_t b = ctx->h[1];
    uint32_t c = ctx->h[2];
    uint32_t d = ctx->h[3];
    uint32_t e = ctx->h[4];
    uint32_t f = ctx->h[5];
    uint32_t g = ctx->h[6];
    uint32_t h = ctx->h[7];

    for (int i = 0; i < 64; i++) {
        uint32_t T1 = h + BSIG1(e) + CH(e, f, g) + K[i] + W[i];
        uint32_t T2 = BSIG0(a) + MAJ(a, b, c);

        h = g;
        g = f;
        f = e;
        e = d + T1;
        d = c;
        c = b;
        b = a;
        a = T1 + T2;
    }

    ctx->h[0] += a;
    ctx->h[1] += b;
    ctx->h[2] += c;
    ctx->h[3] += d;
    ctx->h[4] += e;
    ctx->h[5] += f;
    ctx->h[6] += g;
    ctx->h[7] += h;
}

static void sha256_init(void *vctx) {
    t_sha256_ctx *ctx = vctx;

    ctx->h[0] = 0x6a09e667;
    ctx->h[1] = 0xbb67ae85;
    ctx->h[2] = 0x3c6ef372;
    ctx->h[3] = 0xa54ff53a;
    ctx->h[4] = 0x510e527f;
    ctx->h[5] = 0x9b05688c;
    ctx->h[6] = 0x1f83d9ab;
    ctx->h[7] = 0x5be0cd19;

    ctx->bitlen = 0;
    ctx->buffer_len = 0;
}

static void sha256_update(void *vctx, const uint8_t *data, size_t len) {
    t_sha256_ctx *ctx = (t_sha256_ctx *)vctx;

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
            sha256_process_block(ctx, ctx->buffer);
            ctx->buffer_len = 0;
        }
    }
}

static void sha256_final(void *vctx, uint8_t *out) {
    t_sha256_ctx *ctx = (t_sha256_ctx *)vctx;

    /* Append 0x80 */
    ctx->buffer[ctx->buffer_len++] = 0x80;

    /* Pad with zeros until 56 bytes (or process and continue) */
    if (ctx->buffer_len > 56) {
        while (ctx->buffer_len < 64)
            ctx->buffer[ctx->buffer_len++] = 0x00;
        sha256_process_block(ctx, ctx->buffer);
        ctx->buffer_len = 0;
    }

    while (ctx->buffer_len < 56)
        ctx->buffer[ctx->buffer_len++] = 0x00;

    /* Append message length in bits as big-endian 64-bit */
    write_be64(ctx->buffer + 56, ctx->bitlen);
    sha256_process_block(ctx, ctx->buffer);

    /* Output digest as big-endian 32-bit words */
    for (int i = 0; i < 8; i++)
        write_be32(out + (size_t)i * 4, ctx->h[i]);
}

static void sha256_print(const uint8_t *digest, const t_global_ctx *gctx, const t_job *job) {
    (void)gctx;
    (void)job;

    /* Simple default: just print hex digest */
    for (int i = 0; i < 32; i++)
        printf("%02x", digest[i]);
    printf("\n");
}

const t_algo g_sha256_algo = {.name = "sha256",
                              .ctx_size = sizeof(t_sha256_ctx),
                              .init = sha256_init,
                              .update = sha256_update,
                              .final = sha256_final,
                              .digest_size = 16,
                              .print = sha256_print};
