/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asyed <asyed@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/05 14:49:32 by asyed             #+#    #+#             */
/*   Updated: 2018/03/05 19:24:13 by asyed            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vm.h"
#include "op.h"

int	init(int champc, char **champv)
{
	void	*arena;

	if (!(arena = init_arena()))
		return (-1);
	while (champc)
	{
		if (read_champion(champv[champc], arena) == -1)
		{
			printf("wow fail\n");
			return (-1);
		}
		else
			printf("Read champion \"%s\" into memory\n", champv[champc]);
		champc--;
	}
	return (0);
}

int	main(int argc, char *argv[])
{
	//Make a library parse flags func.
	if (argc < 2)
	{
		printf("No champions listed\n");
		return (-1);
	}
	if (init(argc - 1, argv) == -1)
	{
		printf("Failed to read.\n");
		return (-1);
	}
	return (0);
}
