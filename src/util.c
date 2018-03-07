/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgardner <stephenbgardner@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/02/18 21:46:04 by sgardner          #+#    #+#             */
/*   Updated: 2018/03/07 07:30:54 by sgardner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>
#include "core.h"

void			fatal_error(char *msg)
{
	write(2, msg, ft_strlen(msg));
	write(2, "\n", 1);
	exit(1);
}

unsigned int	reverse_bytes(unsigned int n, ssize_t size)
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
	return (*(unsigned int *)res);
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
