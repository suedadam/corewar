/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   op_aff.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asyed <asyed@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/06 07:24:00 by asyed             #+#    #+#             */
/*   Updated: 2018/03/14 18:06:49 by asyed            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "vm.h"

int	op_aff(t_operation *cmd_input, void *arena, t_process *child)
{
	int	c;

	c = child->regs[(cmd_input->args)[0]] % 256;
	write(1, &c, 1);
	UNUSED(arena);
	return (0);
}
