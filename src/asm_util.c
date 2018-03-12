/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   asm_util.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgardner <stephenbgardner@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/02/18 21:46:04 by sgardner          #+#    #+#             */
/*   Updated: 2018/03/11 21:17:31 by sgardner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>
#include "core.h"
#include "ft_printf.h"

void			fatal_error(char *msg)
{
	ft_printf("%&s %s\n", "1;31m", "ERROR:", msg);
	exit(1);
}

void			lexical_error(int line, int col)
{
	ft_printf("[%03d:%03d] %&s Lexical error\n", line, col, "1;31m", "ERROR:");
	exit(1);
}

int				reverse_bytes(int n, ssize_t size)
{
	char	*bytes;
	char	res[4];
	int		i;

	i = 0;
	bytes = (char *)&n;
	ft_memset(res, 0, 4);
	while (i < size)
	{
		res[i] = bytes[size - i - 1];
		++i;
	}
	return (*(int *)res);
}

void			syntax_error(int line, int col, char *type, char *val)
{
	ft_printf("[%03d:%03d] %&s Syntax error [%s]", line, col, "1;31m", "ERROR:",
		type);
	if (*val)
		ft_printf(" \"%s\"", val);
	ft_printf("\n");
	exit(1);
}

int				trim(char *arg, char *delim)
{
	char	*tmp;
	int		off;
	int		len;

	off = 0;
	while (arg[off] && ft_strchr(delim, arg[off]))
		++off;
	tmp = arg + off;
	if ((len = ft_strlen(tmp)) > 0)
		--len;
	while (tmp[len] && ft_strchr(delim, tmp[len]))
		--len;
	ft_memmove(arg, tmp, ++len);
	arg[len] = '\0';
	return (off);
}
