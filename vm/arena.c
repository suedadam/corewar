/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arena.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asyed <asyed@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/05 19:11:42 by asyed             #+#    #+#             */
/*   Updated: 2018/03/06 03:07:51 by asyed            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vm.h"

void	*init_arena(void)
{
	void	*arena;

	if (!(arena = ft_memalloc(MEM_SIZE)))
		return (NULL);
	return (arena);
}
