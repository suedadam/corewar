/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   flags.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asyed <asyed@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/12 18:15:34 by asyed             #+#    #+#             */
/*   Updated: 2018/03/13 13:16:58 by asyed            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "test.h"

void	*fetch_result(char c, uint8_t convint)
{
	int			i;
	static int	ret = 0;

	i = 0;
	while (g_flags[i].idchar)
	{
		if (c == g_flags[i].idchar)
		{
			if (!convint)
				return (g_flags[i].result);
			else
			{
				ret = atoi(g_flags[i].result); //Change to ft_
				return (&ret);
			}
		}
		i++;
	}
	return (NULL);
}

static int	is_flag(char *str, char *result)
{
	int				i;
	static uint8_t 	toggle = 1;

	i = 0;
	while (g_flags[i].idchar)
	{
		if (str[1] == g_flags[i].idchar)
		{
			if (g_flags[i].valafter)
			{
				if (!result)
					return (-1);
				g_flags[i].result = result;
				return (1);
			}
			else
				g_flags[i].result = &toggle;
			break ;
		}
		i++;
	}
	return (0);
}

int 	flag_parse(int *argc, char ***argv)
{
	char 	**newargv;
	int		i;
	int		ret;

	newargv = &((*argv)[1]);
	i = 0;
	while (newargv[i] && i < *argc)
	{
		if (*(newargv[i]) == '-' && strlen(newargv[i]) > 1)
		{
			if ((ret = is_flag(newargv[i], newargv[i + 1])) == -1)
				return (-1);
			else if (ret == 1)
				i++;
			i++;
		}
		else
			break ;
	}
	*argv = &newargv[i];
	*argc -= i + 1;
	return (0);
}
