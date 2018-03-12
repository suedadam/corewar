/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   op_lld.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asyed <asyed@student.42.us.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/06 07:24:00 by asyed             #+#    #+#             */
/*   Updated: 2018/03/12 01:23:30 by asyed            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vm.h"

int	op_lld(t_operation *cmd_input, void *arena, uint8_t plid, t_process *child)
{
	unsigned char	byte;
	int				val;
	int				test;

	byte = cmd_input->encbyte;
	val = 0;
	printf("ld ");
	if ((byte & 0xC0) == (unsigned char)SHIFT_T_DIR)
	{
		// printf("(%zu) {ld} I'm a dir!!! %02x %d (PC: %lld)\n", g_taskmanager->currCycle, cmd_input->encbyte, (cmd_input->args)[0], child->pc);
		val = (cmd_input->args)[0];
		// copy_memory_fwd_off(&val, arena, MEM_WARP(child + (cmd_input->args[0]) % IDX_MOD), REG_SIZE, )
	}
	else if ((byte & 0xC0) == (unsigned char)SHIFT_T_IND)
	{
		copy_memory_fwd_off(&val, arena,
				MEM_WARP((child->pc + (short)(cmd_input->args)[0])), F_IND_SIZE);
		val = ntohs(val);
		// printf("(%zu) {ld} Val = %02x\n", g_taskmanager->currCycle, val);
	}
	printf("%d r%d\n", cmd_input->args[0], cmd_input->args[1]);
	// test = 0;
	// copy_memory_fwd_off(&test, arena, MEM_WARP(child->pc + val % IDX_MOD), REG_SIZE);
	// test = ntohl(test);
	child->regs[(cmd_input->args)[1]] = val;
	// printf("(%zu) {ld} Into Reg: PC %lld -> %lld Val[%d]: %d\n", g_taskmanager->currCycle, child->pc, MEM_WARP(child->pc + val % IDX_MOD), cmd_input->args[1], child->regs[(cmd_input->args)[1]]);
	if (!(child->regs[(cmd_input->args)[1]]))
		child->carry = 1;
	else
		child->carry = 0;
	// printf("(%zu) {ld} RegVal: %d Carry: %d\n", g_taskmanager->currCycle, (child->regs[(cmd_input->args)[1]]), child->carry);
	return (0);
}
