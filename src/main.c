/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlu <dlu@student.42berlin.de>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 07:27:36 by dlu               #+#    #+#             */
/*   Updated: 2025/12/29 21:39:04 by dlu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// #include "command.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h> // Switch to libft later

typedef enum e_input_type { I_STDIN, I_STRING, I_FILE } t_input_type;

typedef struct s_job {
    const char *input; // filename or string, NULL for stdin
    t_input_type type;
    struct s_job *next;
} t_job;

typedef struct s_global_ctx {
    const char *command; // which algorithm (md5, sha256, ...)
    uint32_t flags;      // -p -q -r
    t_job *jobs;         // linked list of hash jobs
    bool error;
    const char *error_msg;
} t_global_ctx;

static int usage(void) {
    char const *msg = "usage: ft_ssl command [flags] [file/string]";
    fprintf(stderr, "%s\n", msg); // Add ft_fprintf to libft, to stderr
    return (1);
}

#define FLAG_P (1 << 0) // print stdin
#define FLAG_Q (1 << 1) // quiet
#define FLAG_R (1 << 2) // reverse
#define FLAG_S (1 << 3) // string

t_job *new_job(t_global_ctx *ctx, char *av) {
    t_job *job = malloc(sizeof(t_job));
    if (!job)
        return (NULL); // Set a flag?

    if (ctx->flags & FLAG_S) {
        job->input = av;
        job->type = I_STRING;
        ctx->flags &= ~FLAG_S;
    } else {
        job->input = av;
        job->type = I_FILE;
    }
    job->next = NULL;
    return job;
}

void parser(int ac, char **av, t_global_ctx *ctx) {
    /* Initialize global context */
    ctx->command = av[1];
    ctx->jobs = NULL;
    ctx->flags = 0;
    ctx->error = false;

    for (int i = 2; i < ac; i++) {
        if (av[i][0] == '-') {
            for (char *opt = av[i] + 1; *opt; opt++) {
                if (*opt == 'p')
                    ctx->flags |= FLAG_P;
                else if (*opt == 'q')
                    ctx->flags |= FLAG_Q;
                else if (*opt == 'r')
                    ctx->flags |= FLAG_R;
                else if (*opt == 's')
                    ctx->flags |= FLAG_S;
                else {
                    ctx->error = true;
                    ctx->error_msg = "Unknown flag";
                    return;
                }
            }
        } else {
            // new_job(ctx, av[i]); don't worry I'll link later
        }
    }
    if (ctx->flags & FLAG_S) {
        ctx->error = true;
        ctx->error_msg = "Missing string";
    }
}

void print_binary(uint32_t x) {
    for (int i = 31; i >= 0; i--) {
        putchar((x >> i) & 1 ? '1' : '0');
        if (i % 4 == 0)
            putchar(' '); // optional grouping
    }
    putchar('\n');
}

int main(int ac, char **av) {
    t_global_ctx ctx;

    if (ac < 2)
        return usage(); // Bonus: read command from STDIN?

    parser(ac, av, &ctx); // Parser: ac, av => s_global_ctx
    // executor(ctx);        // Everything will fail here, or even help
    printf("%s\n", ctx.command);
    print_binary(ctx.flags);
    return (0);
}
