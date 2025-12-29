/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlu <dlu@student.42berlin.de>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 07:27:36 by dlu               #+#    #+#             */
/*   Updated: 2025/12/29 22:22:37 by dlu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// #include "libft.h"
#include "parser.h"
#include <stdio.h> // Switch to libft later

static int usage(void) {
    char const *msg = "usage: ft_ssl command [flags] [file/string]";
    fprintf(stderr, "%s\n", msg); // Add ft_fprintf to libft, to stderr
    return (1);
}

int main(int ac, char **av) {
    t_global_ctx ctx;

    if (ac < 2)
        return usage(); // Bonus: read command from STDIN?

    parser(ac, av, &ctx); // Parser: ac, av => s_global_ctx
    if (ctx.error) {
        fprintf(stderr, "%s\n", ctx.error_msg);
        return (1);
    }
    // executor(ctx);        // Everything will fail here, or even help
    return (0);
}
