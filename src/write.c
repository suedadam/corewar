/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   write.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgardner <stephenbgardner@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/09 22:38:13 by sgardner          #+#    #+#             */
/*   Updated: 2018/03/10 21:48:22 by sgardner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include "core.h"

static int	label_offset(t_parse *parse, int offset)
{
	static int	data;
	t_token		*curr;

	data = 0;
	curr = parse->tokens;
	while (curr)
	{
		if (curr->type == SYM_LABEL
			&& !ft_strcmp(parse->curr->data, curr->data))
		{
			data = curr->cbyte - offset;
			break ;
		}
		curr = curr->next;
	}
	return (data);
}

static void	write_token(t_parse *parse, int offset, int fd)
{
	t_token	*curr;
	int		data;

	curr = parse->curr;
	if (curr->type == SYM_OP)
	{
		write(fd, curr->data, 1);
		if (curr->len > 1)
			write(fd, &curr->cbyte, 1);
		return ;
	}
	if (curr->type == SYM_DLABEL || curr->type == SYM_INDLABEL)
	{
		data = label_offset(parse, offset);
	}
	else
		data = *(int *)curr->data;
	if (curr->len > 1)
		data = reverse_bytes(data, curr->len);
	write(fd, &data, curr->len);
}

void		write_file(t_parse *parse, char *file)
{
	int	fd;
	int	offset;
	int	last_op;

	offset = 0;
	if ((fd = open(file, O_WRONLY | O_CREAT, 0600)) < 0)
		DEFAULT_ERROR;
	parse->header.size = reverse_bytes(parse->header.size, sizeof(int));
	write(fd, &parse->header, sizeof(t_header));
	parse->curr = parse->tokens;
	while (parse->curr)
	{
		if (parse->curr->type == SYM_OP)
			last_op = offset;
		if (parse->curr->type != UNDEFINED
			&& parse->curr->type > SYM_COMMAND_COMMENT
			&& parse->curr->type != SYM_LABEL)
		{
			write_token(parse, last_op, fd);
			offset += parse->curr->len;
		}
		parse->curr = parse->curr->next;
	}
	close(fd);
}
