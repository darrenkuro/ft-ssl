/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlu <dlu@student.42berlin.de>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 07:27:36 by dlu               #+#    #+#             */
/*   Updated: 2025/12/28 13:57:42 by dlu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int usage(void) {
    char const *msg = "usage: ft_ssl command [flags] [file/string]";
    write(2, msg, ft_strlen(msg));
    return (1);
}

int main(int ac, char **av) {
    if (ac < 2 || !av)
        return usage();
    return 1;
    // return run_cmd(ac - 1, av + 1);
}
