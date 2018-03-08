/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fsm_op.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgardner <stephenbgardner@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/08 01:44:41 by sgardner          #+#    #+#             */
/*   Updated: 2018/03/08 07:54:34 by sgardner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "core.h"
#include "ft_printf.h"

const t_op	g_ops[] = {
	{ "live", OP_LIVE, 1, 1, { T_DIR }, FALSE, FALSE },
	{ "ld", OP_LD, 2, 2, { T_DIR | T_IND, T_REG }, TRUE, FALSE },
	{ "st", OP_ST, 3, 2, { T_REG, T_IND | T_REG }, TRUE, FALSE },
	{ "add", OP_ADD, 4, 3, { T_REG, T_REG, T_REG }, TRUE, FALSE },
	{ "sub", OP_SUB, 5, 3, { T_REG, T_REG, T_REG }, TRUE, FALSE },
	{ "and", OP_AND, 6, 3,
		{ T_REG | T_DIR | T_IND, T_REG | T_IND | T_DIR, T_REG }, TRUE, FALSE },
	{ "or", OP_OR, 7, 3,
		{ T_REG | T_IND | T_DIR, T_REG | T_IND | T_DIR, T_REG }, TRUE, FALSE },
	{ "xor", OP_XOR, 8, 3,
		{ T_REG | T_IND | T_DIR, T_REG | T_IND | T_DIR, T_REG }, TRUE, FALSE },
	{ "zjmp", OP_ZJMP, 9, 1, { T_DIR }, FALSE, TRUE },
	{ "ldi", OP_LDI, 10, 3, { T_REG | T_DIR | T_IND, T_DIR | T_REG, T_REG },
		TRUE, TRUE },
	{ "sti", OP_STI, 11, 3, { T_REG, T_REG | T_DIR | T_IND, T_DIR | T_REG },
		TRUE, TRUE },
	{ "fork", OP_FORK, 12, 1, { T_DIR }, FALSE, TRUE },
	{ "lld", OP_LLD, 13, 2, { T_DIR | T_IND, T_REG }, TRUE, FALSE },
	{ "lldi", OP_LLDI, 14, 3, { T_REG | T_DIR | T_IND, T_DIR | T_REG, T_REG },
		TRUE, TRUE },
	{ "lfork", OP_LFORK, 15, 1, { T_DIR }, FALSE, TRUE },
	{ "aff", OP_AFF, 16, 1, { T_REG }, TRUE, FALSE },
	{ 0, 0, 0, 0, { 0 }, FALSE, FALSE }
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
	int		arg;
	int		i;

	i = 0;
	tok = parse->curr;
	while (g_ops[i].name)
	{
		if (!ft_strcmp(tok->data, g_ops[i].name))
			break ;
		i++;
	}
	if (!g_ops[i].name)
		invalid_op(tok);
	tok->data = (char *)&g_ops[i].id;
	arg = 0;
	while (arg++ < g_ops[i].num_args)
		parse->curr = parse->curr->next;
	return (BUILD_OP);
}
