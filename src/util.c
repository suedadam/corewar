/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgardner <stephenbgardner@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/02/18 21:46:04 by sgardner          #+#    #+#             */
/*   Updated: 2018/03/10 20:08:14 by sgardner         ###   ########.fr       */
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
	ft_printf("%&s Lexical error at [%03d:%03d]\n",
		"1;31m", "ERROR:", line, col);
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
	ft_printf("%&s Syntax error at token [TOKEN][%03d:%03d] %s",
		"1;31m", "ERROR:", line, col, type);
	if (*val)
		ft_printf(" \"%s\"", val);
	ft_printf("\n");
	exit(1);
}

#define IS_WSPACE(x,i)	x[i] && (x[i] == ' ' || x[i] == '\t')

int				trim(char *arg)
{
	char	*tmp;
	int		off;
	int		len;

	off = 0;
	while (IS_WSPACE(arg, off))
		++off;
	tmp = arg + off;
	if ((len = ft_strlen(tmp)))
		--len;
	while (IS_WSPACE((tmp), len))
		--len;
	ft_memmove(arg, tmp, ++len);
	arg[len] = '\0';
	return (off);
}
