/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlu <dlu@student.42berlin.de>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/29 19:57:37 by dlu               #+#    #+#             */
/*   Updated: 2025/12/29 22:20:38 by dlu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

static inline void set_error(t_global_ctx *ctx, const char *msg) {
    ctx->error = true;
    ctx->error_msg = msg;
}

static void add_job(t_global_ctx *ctx, char *av) {
    t_job *job = malloc(sizeof(t_job));

    if (!job) {
        set_error(ctx, E_MALLOC);
        return (NULL);
    }

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
                    set_error(ctx, E_FLAG);
                    return;
                }
            }
        } else {
            add_job(ctx, av[i]);
        }
    }
    if (ctx->flags & FLAG_S)
        set_error(ctx, E_MISSING_S);
}
