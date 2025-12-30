/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlu <dlu@student.42berlin.de>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/29 19:57:37 by dlu               #+#    #+#             */
/*   Updated: 2025/12/30 05:26:08 by dlu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include <string.h> // switch to libft

typedef struct s_command {
    const char *name;
    const t_algo *algo;
} t_command;

extern const t_algo g_md5_algo;

static const t_command g_commands[] = {{"md5", &g_md5_algo}, {"sha256", NULL}, {NULL, NULL}};

// MD5 works too? make it case-insensitive?

static inline void set_error(t_global_ctx *ctx, const char *msg) {
    ctx->error = true;
    ctx->error_msg = msg;
}

static void add_job(t_global_ctx *ctx, char *av) {
    t_job *job = malloc(sizeof(t_job));

    if (!job)
        set_error(ctx, E_MALLOC);

    if (ctx->flags & FLAG_S) {
        job->input = av;
        job->type = I_STRING;
        ctx->flags &= ~FLAG_S;
    } else {
        job->input = av;
        job->type = I_FILE;
    }

    ft_lstadd_back(&ctx->jobs, ft_lstnew(job));
}

static void add_job_stdin(t_global_ctx *ctx) {
    // Default job
    t_job *job = malloc(sizeof(t_job));

    if (!job)
        set_error(ctx, E_MALLOC);

    job->input = "stdin";
    job->type = I_STDIN;
    ft_lstadd_back(&ctx->jobs, ft_lstnew(job));
}

void parser(int ac, char **av, t_global_ctx *ctx) {

    for (int i = 0; g_commands[i].name; i++) {
        if (strcmp(ctx->command, g_commands[i].name) == 0)
            ctx->algo = g_commands[i].algo;
    }
    if (!ctx->algo)
        set_error(ctx, "ugh");

    for (int i = 2; i < ac; i++) {
        if (av[i][0] == '-') {
            for (char *opt = av[i] + 1; *opt; opt++) {
                if (*opt == 'p') {
                    add_job_stdin(ctx);
                    ctx->flags |= FLAG_P;
                } else if (*opt == 'q')
                    ctx->flags |= FLAG_Q;
                else if (*opt == 'r')
                    ctx->flags |= FLAG_R;
                else if (*opt == 's')
                    ctx->flags |= FLAG_S;
                else {
                    set_error(ctx, E_FLAG);
                    return;
                }
            }
        } else {
            add_job(ctx, av[i]);
        }
    }

    if (ft_lstsize(ctx->jobs) == 0)
        add_job_stdin(ctx);

    if (ctx->flags & FLAG_S)
        set_error(ctx, E_MISSING_S);
}
