/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgardner <stephenbgardner@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/05 19:06:25 by sgardner          #+#    #+#             */
/*   Updated: 2018/03/07 12:46:10 by sgardner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "core.h"

void			destroy_tokens(t_token *tokens)
{
	t_token *next;

	while (tokens)
	{
		next = tokens->next;
		free(tokens);
		tokens = next;
	}
}

static t_token	*create_token(char *data, t_symbol type)
{
	t_token	*tok;

	if (!(tok = ft_memalloc(sizeof(t_token))))
		DEFAULT_ERROR;
	tok->data = data;
	tok->type = type;
	return (tok);
}

static t_token	*split(char *data, char *delim, int line_num, int col_num)
{
	t_token	**curr;
	t_token	*head;
	int		i;

	i = 0;
	head = NULL;
	curr = &head;
	while (data[i])
	{
		while (ft_strchr(delim, data[i]))
			data[i++] = '\0';
		if (data[i])
		{
			*curr = create_token(data + i, UNDEFINED);
			(*curr)->line_num = line_num;
			(*curr)->col_num = col_num + trim((*curr)->data) + i + 1;
			if (*(*curr)->data)
				curr = &(*curr)->next;
			else
				ft_memdel((void **)curr);
		}
		while (data[i] && !ft_strchr(delim, data[i]))
			++i;
	}
	return (head);
}

t_token			*tokenize(t_line *lines, char *delim)
{
	t_token **curr;
	t_token	*head;
	t_token	*tokens;

	head = NULL;
	curr = &head;
	while (lines)
	{
		tokens = split(lines->data, delim, lines->line_num, lines->col_num - 1);
		if (tokens)
			*curr = tokens;
		while (*curr)
			curr = &(*curr)->next;
		lines = lines->next;
	}
	return (head);
}
