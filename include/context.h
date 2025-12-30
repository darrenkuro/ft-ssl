/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   context.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlu <dlu@student.42berlin.de>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/29 22:13:00 by dlu               #+#    #+#             */
/*   Updated: 2025/12/30 04:30:46 by dlu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONTEXT_H
#define CONTEXT_H

#include "libft.h" // t_list
#include <stdbool.h>
#include <stdint.h>

typedef enum e_input_type { I_STDIN, I_STRING, I_FILE } t_input_type;

typedef struct s_global_ctx t_global_ctx;
typedef struct s_algo t_algo;
typedef struct s_job t_job;

typedef struct s_job {
    const char *input; // filename or string, "stdin" for stdin
    t_input_type type;
} t_job;

typedef struct s_algo {
    const char *name;

    size_t ctx_size; // allocation

    void (*init)(void *ctx);
    void (*update)(void *ctx, const uint8_t *data, size_t len);
    void (*final)(void *ctx, uint8_t *out);

    size_t digest_size;
    void (*print)(const uint8_t *digest, const t_global_ctx *gctx, const t_job *job);
} t_algo;

typedef struct s_global_ctx {
    const char *command; // algorithm string
    const t_algo *algo;
    uint32_t flags; // bitmask for flags
    t_list *jobs;   // linked list of jobs
    bool error;
    const char *error_msg;
} t_global_ctx;

#endif
