/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   context.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlu <dlu@student.42berlin.de>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/29 22:13:00 by dlu               #+#    #+#             */
/*   Updated: 2025/12/29 22:15:27 by dlu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONTEXT_H
#define CONTEXT_H

#include "libft.h" // t_list
#include <stdbool.h>
#include <stdint.h>

typedef enum e_input_type { I_STDIN, I_STRING, I_FILE } t_input_type;

typedef struct s_job {
    const char *input; // filename or string, NULL for stdin
    t_input_type type;
} t_job;

typedef struct s_global_ctx {
    const char *command; // algorithm string
    uint32_t flags;      // bitmask for flags
    t_list *jobs;        // linked list of jobs
    bool error;
    const char *error_msg;
} t_global_ctx;

#endif
