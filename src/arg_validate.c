/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arg_validate.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgardner <stephenbgardner@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/08 21:23:25 by sgardner          #+#    #+#             */
/*   Updated: 2018/03/09 02:06:45 by sgardner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "core.h"

#define IS_DIGIT(x) ((unsigned int)(x - '0') < 10)

static int		*cw_atoi(t_token *tok)
{
	char	*data;
	int		*n;
	int		sign;
	int		i;

	i = 0;
	data = tok->data;
	if (!(n = ft_memalloc(sizeof(int))))
		DEFAULT_ERROR;
	sign = (data[i] == '-') ? -1 : 1;
	if (data[i] == '-')
		++i;
	while (data[i])
	{
		if (!IS_DIGIT(data[i]))
		{
			free(n);
			syntax_error(tok->line_num, tok->col_num + i, "INSTRUCTION",
					&data[i]);
		}
		*n = (*n * 10) + ((data[i++] - '0') * sign);
	}
	return (n);
}

t_bool			read_reg(t_token *arg)
{
	ft_memmove(arg->data, arg->data + 1, arg->len--);
	arg->data = (char *)cw_atoi(arg);
	if (*arg->data < 1 || *arg->data > 16)
	{
		free(arg->data);
		return (FALSE);
	}
	arg->type = SYM_REGISTER;
	return (TRUE);
}

static void		validate_label(t_token *arg)
{
	int		i;

	i = 0;
	ft_memmove(arg->data, arg->data + 1, arg->len--);
	++arg->col_num;
	while (arg->data[i])
	{
		if (!ft_strchr(LABEL_CHARS, arg->data[i]))
			lexical_error(arg->line_num, arg->col_num + i);
		++i;
	}
}

t_bool			read_direct(t_token *arg)
{
	ft_memmove(arg->data, arg->data + 1, arg->len--);
	++arg->col_num;
	if (*arg->data == ':')
	{
		validate_label(arg);
		arg->type = SYM_DLABEL;
		return (TRUE);
	}
	arg->data = (char *)cw_atoi(arg);
	arg->type = SYM_DIRECT;
	return (TRUE);
}

t_bool			read_indirect(t_token *arg)
{
	int	*res;

	if (*arg->data == ':')
	{
		validate_label(arg);
		arg->type = SYM_INDLABEL;
		return (TRUE);
	}
	res = cw_atoi(arg);
	*res = (short)*res;
	arg->data = (char *)res;
	arg->type = SYM_IND;
	return (TRUE);
}
