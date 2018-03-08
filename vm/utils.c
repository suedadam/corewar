/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asyed <asyed@student.42.us.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/05 19:11:42 by asyed             #+#    #+#             */
/*   Updated: 2018/03/07 04:24:21 by asyed            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vm.h"

void rev_write_memory(void *arena, unsigned char *src, int offset, int size)
{
	unsigned char	*dst;

	offset = (MEM_SIZE + offset - 1) % MEM_SIZE;
	dst = arena;
	while (size--)
	{
		if (offset > (MEM_SIZE - 1))
			offset = 0;
		*(dst + offset++) = *src++;
	}
}

void	write_memory(void *arena, unsigned char *src, int offset, int size)
{
	if (offset > (MEM_SIZE - 1))
		offset = offset % MEM_SIZE;
	while (size--)
	{
		if (offset > (MEM_SIZE - 1))
			offset = 0;
		*((unsigned char *)arena + offset++) = *src++;
	}
}

void	test_copy_memory_fwd_off(void *dst, unsigned char *src, int offset, int size)
{
	if (offset > (MEM_SIZE - 1))
		offset = offset % MEM_SIZE;
	while (size--)
	{
		if (offset > (MEM_SIZE - 1))
			offset = 0;
		*((unsigned char *)dst++) = *(src + offset++);  
	}
}

int	copy_memory_fwd_off(void *dest, unsigned char *arena, t_process *child, size_t size, int offset)
{
	int		frag;
	int		memloc;
	void	*new;

	frag = 0;
	memloc = child->pc + offset;
	if (memloc >= (MEM_SIZE - 1))
		memloc = (memloc + offset) % (MEM_SIZE - 1);
	new = arena + memloc;
	if ((memloc + size) >= (MEM_SIZE - 1))
		frag = size = ((memloc + size) % (MEM_SIZE - 1));
	if (frag)
	{
		memcpy(dest, new, frag);
		new = arena;
	}
	memcpy(dest, new + frag, size - frag);
	return (0);
}

/*
** I need to handle negative seeks............ lame
*/

void	*ft_memory_warp(void *arena, uint64_t base, uint64_t seek, uint64_t size,
	int *frag)
{
	void	*new;

	if (base + seek >= (MEM_SIZE - 1))
	{
		new = (arena + (seek - ((uint64_t)(MEM_SIZE - 1) - base)));
		return (new);
	}
	else
	{
		if (base + seek + size >= (MEM_SIZE - 1))
			*frag = ((base + seek + size) % (MEM_SIZE - 1));
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
		new = arena + ((MEM_SIZE - 1) + (base + seek));
		if (((MEM_SIZE - 1) + (base + seek)) >= (MEM_SIZE - 1))
		// if ((tmp = (MEM_SIZE + (base + seek) + size) % MEM_SIZE))
		{
			printf("Size = %d tmp = %d\n", size, tmp);
			*frag = (MEM_SIZE - 1) - ((MEM_SIZE - 1) + (base + seek));
		}
	}
	return (new);
}
