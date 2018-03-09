/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fsm_label.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgardner <stephenbgardner@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/08 01:45:45 by sgardner          #+#    #+#             */
/*   Updated: 2018/03/09 01:14:57 by sgardner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "core.h"
#include "ft_printf.h"

t_state		fsm_build_label(t_parse *parse)
{
	t_token	*tok;
	t_token	*curr;
	int		i;

	i = 0;
	tok = parse->curr;
	while (i < tok->len)
	{
		if (!ft_strchr(LABEL_CHARS, tok->data[i]))
			lexical_error(tok->line_num, tok->col_num + i);
		++i;
	}
	curr = parse->tokens;
	while (curr != tok)
	{
		if (curr->type == SYM_LABEL && !ft_strcmp(curr->data, tok->data))
		{
			ft_printf("Duplicate label at [%03d:%03d]\n",
				tok->line_num, tok->col_num);
			exit(1);
		}
		curr = curr->next;
	}
	return (BUILD_LABEL);
}
