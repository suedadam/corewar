/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   asm_load.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asyed <asyed@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/06 18:57:42 by sgardner          #+#    #+#             */
/*   Updated: 2018/03/14 20:58:55 by asyed            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include "core.h"
#include "get_next_line.h"

static char		*strip_comments(char *arg)
{
	t_bool	comment;
	int		i;

	i = 0;
	comment = FALSE;
	while (arg[i])
	{
		if (comment)
			arg[i] = '\0';
		else if (arg[i] == COMMENT_CHAR)
			comment = TRUE;
		else
			++i;
	}
	return (arg);
}

static void		read_file(t_line **head, int fd)
{
	char	*line;
	int		row;
	int		res;

	row = 1;
	while ((res = get_next_line(fd, &line)) > 0)
	{
		if (!(*head = ft_memalloc(sizeof(t_line))))
			DEFAULT_ERROR;
		(*head)->row = row++;
		if (*line)
		{
			(*head)->data = strip_comments(line);
			(*head)->col = trim(line, " \t\n") + 1;
			if (*(*head)->data)
			{
				head = &(*head)->next;
				continue ;
			}
		}
		free(line);
		ft_memdel((void **)head);
	}
	if (res < 0)
		DEFAULT_ERROR;
}

t_line			*load_file(char *file)
{
	t_line	*head;
	int		fd;

	if ((fd = open(file, O_RDONLY)) < 0)
		DEFAULT_ERROR;
	head = NULL;
	read_file(&head, fd);
	close(fd);
	if (!head)
		fatal_error("Empty or invalid input file");
	return (head);
}

void			unload_lines(t_line *lines)
{
	t_line	*next;

	while (lines)
	{
		free(lines->data);
		next = lines->next;
		free(lines);
		lines = next;
	}
}
