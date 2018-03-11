/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fsm_header.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgardner <stephenbgardner@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/07 22:13:10 by sgardner          #+#    #+#             */
/*   Updated: 2018/03/11 14:18:23 by sgardner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "core.h"
#include "ft_printf.h"

static char	*build_val(t_token *start, t_token *end, int len)
{
	char	*val;
	char	*tmp;
	t_token	*curr;

	if (!(val = ft_memalloc(len + 1)))
		DEFAULT_ERROR;
	tmp = val;
	curr = start;
	while (TRUE)
	{
		tmp = ft_stpcpy(tmp, curr->data);
		if (curr == end)
			return (val);
		else
			*tmp++ = ' ';
		curr = curr->next;
	}
}

static void	missing_param(t_token *tok)
{
	syntax_error(tok->row, tok->col + tok->len, "ENDLINE", "");
}

static char	*get_val(t_parse *parse, int max_len)
{
	t_token	*curr;
	t_token	*start;
	t_token	*end;
	int		len;

	len = 0;
	curr = parse->curr;
	if (*(char *)curr->data != '\"')
		lexical_error(curr->row, curr->col);
	start = curr;
	ft_memmove(start->data, start->data + 1, start->len--);
	while (TRUE)
	{
		len += curr->len + (curr->next != NULL);
		if (!curr->next)
			missing_param(curr);
		if (((char *)curr->data)[curr->len - 1] == '\"')
			break ;
		curr = curr->next;
	}
	end = curr;
	((char *)end->data)[end->len - 1] = '\0';
	parse->curr = end;
	len -= 2;
	return ((len > max_len) ? NULL : build_val(start, end, len));
}

static void	oversized(char *type, int max_len)
{
	ft_printf("%&s Champion %s too long (Max length %d)\n", "1;31m", "ERROR:",
		type, max_len);
	exit(1);
}

t_state		fsm_build_header(t_parse *parse)
{
	char	*data;
	char	*val;
	int		max_len;

	if (parse->curr->type == SYM_COMMAND_NAME)
	{
		data = parse->header.name;
		max_len = PROG_NAME_LENGTH;
	}
	else
	{
		data = parse->header.desc;
		max_len = COMMENT_LENGTH;
	}
	if (*data)
		fsm_syntax_error(parse);
	if (!parse->curr->next)
		missing_param(parse->curr);
	parse->curr = parse->curr->next;
	if (!(val = get_val(parse, max_len)))
		oversized((data == parse->header.name) ? "name" : "comment", max_len);
	ft_stpcpy(data, val);
	free(val);
	return (BUILD_HEADER);
}
