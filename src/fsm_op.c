/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fsm_op.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgardner <stephenbgardner@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/08 01:44:41 by sgardner          #+#    #+#             */
/*   Updated: 2018/03/10 13:48:11 by sgardner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "core.h"
#include "ft_printf.h"

const t_op		g_ops[] = {
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

static void		invalid_op(t_token *tok)
{
	ft_printf("%&s %s[%03d:%03d] INSTRUCTION \"%s\"\n",
		"1;31m", "ERROR:", "Invalid instruction at token [TOKEN]",
		tok->line_num, tok->col_num, tok->data);
	exit(1);
}

static void		invalid_param(t_token *op, t_token *arg, int i)
{
	ft_printf("%&s %s %d [%03d:%03d] for instruction \"%s\"\n",
		"1;31m", "ERROR:", "Invalid parameter", i + 1, arg->line_num,
		arg->col_num, op->data);
	exit(1);
}

static t_token	*validate_params(t_parse *parse, t_token *tok, const t_op *op)
{
	t_token	*arg;
	char	*data;
	int		i;

	i = 0;
	arg = tok;
	while (i < op->num_args)
	{
		arg = arg->next;
		data = arg->data;
		if (((op->allowed[i] & T_REG) && *data == 'r' && read_reg(tok, arg))
			|| ((op->allowed[i] & T_DIR) && *data == '%'
				&& read_direct(tok, arg, op->truncate))
			|| ((op->allowed[i] & T_IND) && read_indirect(tok, arg)))
		{
			++i;
			parse->header.size += arg->len;
			continue ;
		}
		invalid_param(tok, arg, i);
	}
	return (arg);
}

t_state			fsm_build_op(t_parse *parse)
{
	t_token		*tok;
	const t_op	*op;

	op = &g_ops[0];
	tok = parse->curr;
	while (op->name)
	{
		if (!ft_strcmp(tok->data, op->name))
			break ;
		++op;
	}
	if (!op->name)
		invalid_op(tok);
	parse->curr = validate_params(parse, tok, op);
	parse->header.size += 1;
	tok->data = (char *)&op->id;
	tok->len = 1;
	return (BUILD_OP);
}
