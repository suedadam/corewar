/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arg_validate.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgardner <stephenbgardner@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/08 21:23:25 by sgardner          #+#    #+#             */
/*   Updated: 2018/03/11 04:38:08 by sgardner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "core.h"

#define IS_DIGIT(x) ((unsigned int)(x - '0') < 10)

static int		*cw_atoi(char *data)
{
	int	*res;
	int	n;
	int	sign;

	n = 0;
	sign = (*data == '-') ? -1 : 1;
	if (*data == '-')
		++data;
	if (!*data)
		return (NULL);
	while (*data)
	{
		if (!IS_DIGIT(*data))
			return (NULL);
		n = (n * 10) + ((*data++ - '0') * sign);
	}
	if (!(res = ft_memalloc(sizeof(int))))
		DEFAULT_ERROR;
	*res = n;
	return (res);
}

t_bool			read_reg(t_token *op, t_token *arg)
{
	int	*res;

	ft_memmove(arg->data, arg->data + 1, arg->len);
	if (!(res = cw_atoi(arg->data)))
		return (FALSE);
	if (*res < 1 || *res > REG_NUMBER)
	{
		free(res);
		return (FALSE);
	}
	arg->data = res;
	arg->type = SYM_REGISTER;
	arg->len = REG_SIZE;
	op->cbyte = (op->cbyte << 2) + REG_CODE;
	return (TRUE);
}

t_bool			read_direct(t_token *op, t_token *arg, t_bool truncate)
{
	int	*res;

	ft_memmove(arg->data, arg->data + 1, arg->len);
	++arg->col;
	if (*(char *)arg->data == LABEL_CHAR)
	{
		ft_memmove(arg->data, arg->data + 1, arg->len);
		++arg->col;
		validate_label_chars(arg);
		arg->type = SYM_DLABEL;
	}
	else
	{
		if (!(res = cw_atoi(arg->data)))
			return (FALSE);
		arg->data = res;
		arg->type = SYM_DIRECT;
	}
	arg->len = (truncate) ? IND_SIZE : DIR_SIZE;
	op->cbyte = (op->cbyte << 2) + DIR_CODE;
	return (TRUE);
}

t_bool			read_indirect(t_token *op, t_token *arg)
{
	int	*res;

	if (*(char *)arg->data == LABEL_CHAR)
	{
		ft_memmove(arg->data, arg->data + 1, arg->len);
		++arg->col;
		validate_label_chars(arg);
		arg->type = SYM_INDLABEL;
	}
	else
	{
		if (!(res = cw_atoi(arg->data)))
			return (FALSE);
		arg->data = res;
		arg->type = SYM_IND;
	}
	arg->len = IND_SIZE;
	op->cbyte = (op->cbyte << 2) + IND_CODE;
	return (TRUE);
}
