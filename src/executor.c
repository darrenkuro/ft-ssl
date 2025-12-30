/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlu <dlu@student.42berlin.de>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/29 22:26:14 by dlu               #+#    #+#             */
/*   Updated: 2025/12/30 05:29:42 by dlu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "context.h"
#include "flags.h"
#include <stdlib.h>
#include <string.h>

#define BUF_SIZE 4096

static int stream_fd(int fd, void *algo_ctx, void (*update)(void *, const uint8_t *, size_t),
                     uint32_t flags) {
    uint8_t buf[BUF_SIZE];
    ssize_t n;
    bool quiet = flags & FLAG_Q;

    if (flags & FLAG_P && !quiet)
        write(STDOUT_FILENO, "(\"", 2);

    while ((n = read(fd, buf, sizeof(buf))) > 0) {
        if (flags & FLAG_P)
            write(STDOUT_FILENO, buf, quiet || buf[n - 1] != '\n' ? n : n - 1);
        update(algo_ctx, buf, (size_t)n);
    }

    if (flags & FLAG_P && !quiet)
        write(STDOUT_FILENO, "\")= ", 4);

    if (n < 0) {
        // perror("read");
        return -1;
    }
    return 0;
}

static int stream_file(const char *path, void *algo_ctx,
                       void (*update)(void *, const uint8_t *, size_t)) {
    int fd = open(path, O_RDONLY);
    if (fd < 0) {
        // perror(path);
        return -1;
    }

    int ret = stream_fd(fd, algo_ctx, update, false);
    close(fd);
    return ret;
}

void executor(t_global_ctx *ctx) {
    uint8_t *digest;
    void *algo_ctx;

    digest = malloc(ctx->algo->digest_size);
    if (!digest) {
        // perror("malloc");
        return;
    }

    for (t_list *node = ctx->jobs; node; node = node->next) {
        t_job *job = node->content;

        algo_ctx = malloc(ctx->algo->ctx_size);
        if (!algo_ctx) {
            // perror("malloc");
            break;
        }

        ctx->algo->init(algo_ctx);

        if (job->type == I_STDIN) {
            if (stream_fd(STDIN_FILENO, algo_ctx, ctx->algo->update, ctx->flags) < 0) {
                free(algo_ctx);
                continue;
            }
        } else if (job->type == I_FILE) {
            if (stream_file(job->input, algo_ctx, ctx->algo->update) < 0) {
                free(algo_ctx);
                continue;
            }
        } else if (job->type == I_STRING) {
            ctx->algo->update(algo_ctx, (const uint8_t *)job->input, strlen(job->input));
        }

        ctx->algo->final(algo_ctx, digest);
        ctx->algo->print(digest, ctx, job);

        free(algo_ctx);
    }

    free(digest);
}
