/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   asm_label.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgardner <stephenbgardner@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/08 01:45:45 by sgardner          #+#    #+#             */
/*   Updated: 2018/03/12 00:53:16 by sgardner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "core.h"
#include "ft_printf.h"

static void	find_duplicate(t_parse *parse, t_token *seek)
{
	t_token	*iter;

	iter = parse->tokens;
	while (iter != seek)
	{
		if (iter->type == SYM_LABEL && !ft_strcmp(iter->data, seek->data))
		{
			ft_printf("[%03d:%03d] %&s Duplicate label\n",
				seek->row, seek->col, "1;31m", "ERROR:");
			exit(1);
		}
		iter = iter->next;
	}
}

t_state		fsm_build_label(t_parse *parse)
{
	t_token	*tok;

	tok = parse->curr;
	validate_label_chars(tok);
	find_duplicate(parse, tok);
	tok->cbyte = parse->header.size;
	return (BUILD_LABEL);
}

void		validate_label_chars(t_token *arg)
{
	char	*data;

	data = arg->data;
	data[--arg->len] = '\0';
	while (*data)
	{
		if (!ft_strchr(LABEL_CHARS, *data))
			lexical_error(arg->row, arg->col + (data - (char *)arg->data));
		++data;
	}
}
