/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asyed <asyed@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/05 19:11:42 by asyed             #+#    #+#             */
/*   Updated: 2018/03/06 14:55:24 by asyed            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vm.h"

/*
** I need to handle negative seeks............ lame
*/

void	*ft_memory_warp(void *arena, uint64_t base, uint64_t seek, uint64_t size,
	int *frag)
{
	void	*new;

	if (base + seek >= MEM_SIZE)
	{
		new = (arena + (seek - ((uint64_t)MEM_SIZE - base)));
		return (new);
	}
	else
	{
		if (base + seek + size >= MEM_SIZE)
			*frag = ((base + seek + size) % MEM_SIZE);
		new = (arena + base + seek);
		return (new);
	}
}

void	*ft_rev_mem_warp(void *arena, int64_t base, int seek, int size,
	int *frag)
{
	void	*new;
	int		tmp;

	if ((base + seek) >= 0)
	{
		//Check forwards space boundary?
		new = (arena + (base + seek));
	}
	else
	{
		new = arena + (MEM_SIZE + (base + seek));
		if ((tmp = (MEM_SIZE + (base + seek) + size) % MEM_SIZE))
			*frag = size - tmp;
	}
	return (new);
}