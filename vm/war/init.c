/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asyed <asyed@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/05 23:42:15 by asyed             #+#    #+#             */
/*   Updated: 2018/03/06 03:37:33 by asyed            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vm.h"

int	init_war(void *arena)
{
	//Lets start this BITCHESSS.'C:'
	int	i;

	i = 0;
	while (1)
	{
		if ((taskmanager->players)[i])
		{
			run_operation(i, arena);
			// printf("playerID = %d\n", (taskmanager->players)[i]->pID);
			i++;
		}
		else
		{
			(taskmanager->currCycle)++;
			i = 0;
		}
	}
}
