/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlu <dlu@student.42berlin.de>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 07:27:36 by dlu               #+#    #+#             */
/*   Updated: 2025/12/29 20:38:12 by dlu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "command.h"
#include <stdint.h>
#include <stdio.h> // Switch to libft later

typedef enum e_input_type { STDIN, STRING, FILE } t_input_type;

typedef struct s_job {
    t_input_type type;
    const char *value; // filename or string, NULL for stdin
    struct s_job *next;
} t_job;

typedef struct s_global_ctx {
    const char *command; // which algorithm (md5, sha256, ...)
    uint32_t flags;      // -p -q -r
    t_job *jobs;         // linked list of hash jobs
} t_global_ctx;

static int usage(void) {
    char const *msg = "usage: ft_ssl command [flags] [file/string]";
    fprintf(stderr, "%s\n", msg); // Add ft_fprintf to libft, to stderr
    return (1);
}

int main(int ac, char **av) {
    t_global_ctx *ctx;

    if (ac < 2)
        return usage(); // Bonus: read command from STDIN?

    parser(ac - 1, av + 1, &ctx); // Parser: ac, av => s_global_ctx
    executor(ctx);

    return (0);
}
