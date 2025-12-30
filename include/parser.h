/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlu <dlu@student.42berlin.de>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/29 22:13:36 by dlu               #+#    #+#             */
/*   Updated: 2025/12/29 22:19:13 by dlu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
#define PARSER_H

#include "context.h"
#include "flags.h"

#define E_MALLOC "Malloc failed"
#define E_FLAG "UNKNOWN_FLAG"
#define E_MISSING_S "Missing string argument after -s"

void parser(int ac, char **av, t_global_ctx *ctx);

#endif
