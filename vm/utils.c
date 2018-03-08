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
	offset = (offset + MEM_SIZE) % MEM_SIZE;
	while (size--)
	{
		if (offset > (MEM_SIZE - 1))
			offset = 0;
		*((unsigned char *)arena + offset++) = *src++;
	}
}

void	write_memory(void *arena, unsigned char *src, int offset, int size)
{
	offset = offset % MEM_SIZE;
	while (size--)
	{
		if (offset > (MEM_SIZE - 1))
			offset = 0;
		*((unsigned char *)arena + offset++) = *src++;
	}
}

void	copy_memory_fwd_off(void *dst, unsigned char *src, int offset, int size)
{
	offset = (offset + MEM_SIZE) % MEM_SIZE;
	while (size--)
	{
		if (offset > (MEM_SIZE - 1))
			offset = 0;
		*((unsigned char *)dst++) = *(src + offset++);  
	}
}
