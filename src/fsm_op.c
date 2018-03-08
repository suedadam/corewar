/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fsm_op.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgardner <stephenbgardner@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/08 01:44:41 by sgardner          #+#    #+#             */
/*   Updated: 2018/03/08 06:36:47 by sgardner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "core.h"
#include "ft_printf.h"

const t_op	g_ops[] = {
	{ "live", 1, 1, OP_LIVE, FALSE, FALSE },
	{ "ld", 2, 2, OP_LD, TRUE, FALSE },
	{ "st", 2, 3, OP_ST, TRUE, FALSE },
	{ "add", 3, 4, OP_ADD, TRUE, FALSE },
	{ "sub", 3, 5, OP_SUB, TRUE, FALSE },
	{ "and", 3, 6, OP_AND, TRUE, FALSE },
	{ "or", 3, 7, OP_OR, TRUE, FALSE },
	{ "xor", 3, 8, OP_XOR, TRUE, FALSE },
	{ "zjmp", 1, 9, OP_ZJMP, FALSE, TRUE },
	{ "ldi", 3, 10, OP_LDI, TRUE, TRUE },
	{ "sti", 3, 11, OP_STI, TRUE, TRUE },
	{ "fork", 1, 12, OP_FORK, FALSE, TRUE },
	{ "lld", 2, 13, OP_LLD, TRUE, FALSE },
	{ "lldi", 3, 14, OP_LLDI, TRUE, TRUE },
	{ "lfork", 1, 15, OP_LFORK, FALSE, TRUE },
	{ "aff", 1, 16, OP_AFF, TRUE, FALSE },
	{ 0, 0, 0, 0, FALSE, FALSE }
};

static void	invalid_op(t_token *tok)
{
	ft_printf("%s[%03d:%03d] INSTRUCTION \"%s\"\n",
		"Invalid instruction at token [TOKEN]",
		tok->line_num, tok->col_num, tok->data);
	exit(1);
}

t_state		fsm_build_op(t_parse *parse)
{
	t_token	*tok;
	int		i;

	i = 0;
	tok = parse->curr;
	while (g_ops[i].name)
	{
		if (!ft_strcmp(tok->data, g_ops[i].name))
			break;
		i++;
	}
	if (!g_ops[i].name)
		invalid_op(tok);
	return (BUILD_OP);
}
