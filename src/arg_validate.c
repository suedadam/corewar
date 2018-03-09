/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arg_validate.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgardner <stephenbgardner@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/08 21:23:25 by sgardner          #+#    #+#             */
/*   Updated: 2018/03/08 23:26:08 by sgardner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "core.h"

#define IS_DIGIT(x) ((unsigned int)(x - '0') < 9)

static int	*cw_atoi(t_token *tok)
{
	char	*data;
	int		*n;
	int		sign;
	int		i;

	i = 1;
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

t_bool		read_reg(t_token *arg)
{
	arg->data = (char *)cw_atoi(arg);
	if (*arg->data < 1 || *arg->data > 16)
	{
		free(arg->data);
		return (FALSE);
	}
	arg->type = SYM_REGISTER;
	return (TRUE);
}

t_bool		read_direct(t_token *arg)
{
	return (arg != NULL);
}

t_bool		read_indirect(t_token *arg)
{
	return (arg != NULL);
}
