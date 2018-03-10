/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arg_validate.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgardner <stephenbgardner@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/08 21:23:25 by sgardner          #+#    #+#             */
/*   Updated: 2018/03/10 13:42:42 by sgardner         ###   ########.fr       */
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
	int	i;

	i = 0;
	n = 0;
	sign = (data[i] == '-') ? -1 : 1;
	if (data[i] == '-')
		++i;
	if (!data[i])
		return (NULL);
	while (data[i])
	{
		if (!IS_DIGIT(data[i]))
			return (NULL);
		n = (n * 10) + ((data[i++] - '0') * sign);
	}
	if (!(res = ft_memalloc(sizeof(int))))
		DEFAULT_ERROR;
	*res = n;
	return (res);
}

t_bool			read_reg(t_token *op, t_token *arg)
{
	ft_memmove(arg->data, arg->data + 1, arg->len--);
	if (!(arg->data = (char *)cw_atoi(arg->data)))
		return (FALSE);
	if (*arg->data < 1 || *arg->data > REG_NUMBER)
	{
		free(arg->data);
		return (FALSE);
	}
	arg->type = SYM_REGISTER;
	arg->len = 4;
	op->cbyte = (op->cbyte << 2) + REG_CODE;
	return (TRUE);
}

static t_bool	validate_label(t_token *arg)
{
	int		i;

	i = 0;
	ft_memmove(arg->data, arg->data + 1, arg->len--);
	while (arg->data[i])
	{
		if (!ft_strchr(LABEL_CHARS, arg->data[i]))
			return (FALSE);
		++i;
	}
	return (TRUE);
}

t_bool			read_direct(t_token *op, t_token *arg, t_bool truncate)
{
	int	*res;

	ft_memmove(arg->data, arg->data + 1, arg->len--);
	if (*arg->data == ':')
	{
		if (!validate_label(arg))
			return (FALSE);
		arg->type = SYM_DLABEL;
	}
	else
	{
		if (!(res = cw_atoi(arg->data)))
			return (FALSE);
		if (truncate)
			*res = (short)*res;
		arg->data = (char *)res;
		arg->type = SYM_DIRECT;
	}
	arg->len = (truncate) ? 2 : 4;
	op->cbyte = (op->cbyte << 2) + DIR_CODE;

	return (TRUE);
}

t_bool			read_indirect(t_token *op, t_token *arg)
{
	int	*res;

	if (*arg->data == ':')
	{
		if (!validate_label(arg))
			return (FALSE);
		arg->type = SYM_INDLABEL;
	}
	else
	{
		if (!(res = cw_atoi(arg->data)))
			return (FALSE);
		*res = (short)*res;
		arg->data = (char *)res;
		arg->type = SYM_IND;
	}
	arg->len = 2;
	op->cbyte = (op->cbyte << 2) + IND_CODE;
	return (TRUE);
}
