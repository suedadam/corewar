/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   asm_op.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgardner <stephenbgardner@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/08 01:44:41 by sgardner          #+#    #+#             */
/*   Updated: 2018/03/12 00:53:31 by sgardner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "core.h"
#include "ft_printf.h"

const t_op		g_ops[] = {
	{ "live", 1, 1, { T_D }, FALSE, FALSE },
	{ "ld", 2, 2, { T_D | T_I, T_R }, TRUE, FALSE },
	{ "st", 3, 2, { T_R, T_I | T_R }, TRUE, FALSE },
	{ "add", 4, 3, { T_R, T_R, T_R }, TRUE, FALSE },
	{ "sub", 5, 3, { T_R, T_R, T_R }, TRUE, FALSE },
	{ "and", 6, 3, { T_R | T_D | T_I, T_R | T_I | T_D, T_R }, TRUE, FALSE },
	{ "or", 7, 3, { T_R | T_I | T_D, T_R | T_I | T_D, T_R }, TRUE, FALSE },
	{ "xor", 8, 3, { T_R | T_I | T_D, T_R | T_I | T_D, T_R }, TRUE, FALSE },
	{ "zjmp", 9, 1, { T_D }, FALSE, TRUE },
	{ "ldi", 10, 3, { T_R | T_D | T_I, T_D | T_R, T_R }, TRUE, TRUE },
	{ "sti", 11, 3, { T_R, T_R | T_D | T_I, T_D | T_R }, TRUE, TRUE },
	{ "fork", 12, 1, { T_D }, FALSE, TRUE },
	{ "lld", 13, 2, { T_D | T_I, T_R }, TRUE, FALSE },
	{ "lldi", 14, 3, { T_R | T_D | T_I, T_D | T_R, T_R }, TRUE, TRUE },
	{ "lfork", 15, 1, { T_D }, FALSE, TRUE },
	{ "aff", 16, 1, { T_R }, TRUE, FALSE },
	{ 0, 0, 0, { 0 }, FALSE, FALSE }
};

static void		invalid_op(t_token *op)
{
	ft_printf("[%03d:%03d] %&s Invalid instruction \"%s\"\n",
		op->row, op->col, "1;31m", "ERROR:", op->data);
	exit(1);
}

static void		invalid_param(t_token *op, t_token *arg, int i)
{
	ft_printf("[%03d:%03d] %&s Invalid parameter %d for instruction \"%s\"\n",
		arg->row, arg->col, "1;31m", "ERROR:", i + 1, op->data);
	exit(1);
}

#define ALLOWED(x) (spec->allowed[i] & x)

static t_token	*validate_params(t_parse *parse, t_token *op, const t_op *spec)
{
	t_token	*arg;
	char	*data;
	int		i;

	i = 0;
	arg = op;
	while (i < spec->num_args)
	{
		arg = arg->next;
		data = arg->data;
		if ((ALLOWED(T_R) && *data == 'r' && read_reg(op, arg))
			|| (ALLOWED(T_D) && *data == '%'
				&& read_direct(op, arg, spec->truncate))
			|| (ALLOWED(T_I) && read_indirect(op, arg)))
		{
			++i;
			parse->header.size += arg->len;
			continue ;
		}
		invalid_param(op, arg, i);
	}
	op->cbyte <<= (8 - (2 * spec->num_args));
	return (arg);
}

t_state			fsm_build_op(t_parse *parse)
{
	t_token		*op;
	const t_op	*spec;

	spec = &g_ops[0];
	op = parse->curr;
	while (spec->name && ft_strcmp(op->data, spec->name))
		++spec;
	if (!spec->name)
		invalid_op(op);
	parse->curr = validate_params(parse, op, spec);
	op->data = (char *)&spec->id;
	op->len = 1 + (spec->coding == TRUE);
	parse->header.size += op->len;
	return (BUILD_OP);
}
