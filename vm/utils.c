/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asyed <asyed@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/05 19:11:42 by asyed             #+#    #+#             */
/*   Updated: 2018/03/06 10:11:17 by asyed            ###   ########.fr       */
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
