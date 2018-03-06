/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strcdup.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asyed <asyed@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/01/04 17:02:00 by asyed             #+#    #+#             */
/*   Updated: 2018/01/04 19:09:59 by asyed            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>

char	*ft_strcdup(const char *line, char c)
{
	char	*dest;
	int		i;

	dest = (char *)malloc(ft_strclen(line, c) + 1);
	if (dest)
	{
		i = 0;
		while (*line && *line != c)
			dest[i++] = *line++;
		dest[i] = '\0';
	}
	return (dest);
}
