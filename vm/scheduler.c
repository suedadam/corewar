/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scheduler.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asyed <asyed@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/05 15:31:46 by asyed             #+#    #+#             */
/*   Updated: 2018/03/05 18:37:40 by asyed            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vm.h"

t_scheduler	scheduler;

int	init_scheduler(void)
{
	t_scheduler *scheduler;

	if (!(scheduler = ft_memalloc(sizeof(t_scheduler))))
	{
		printf("scheduler = ft_memalloc(%s)\n", strerror(errno));
		return (-1);
	}
	return (0);
}

/*
** The scheduler should cache the op_code and seek to the PC (memory location)
** when the schedule completes and then executes with the input provided (
** even if overwritten with a different op_code. The cached one is executed.)
** 
** If invalid then calculate the seek distance based on the Encoding byte
** otherwise 
*/

int	add_scheduler(uint8_t opcode)
{
	return (0);
}
