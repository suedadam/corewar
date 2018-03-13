/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   int_handlers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgardner <stephenbgardner@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/09 18:31:17 by sgardner          #+#    #+#             */
/*   Updated: 2017/10/15 14:16:07 by sgardner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int			print_d(t_arg *arg)
{
	return (print_int(arg, 10, TRUE));
}

int			print_o(t_arg *arg)
{
	return (print_int(arg, 8, FALSE));
}

int			print_p(t_arg *arg)
{
	arg->flags |= F_HASH;
	return (print_int(arg, 16, FALSE));
}

int			print_u(t_arg *arg)
{
	return (print_int(arg, 10, FALSE));
}

int			print_x(t_arg *arg)
{
	return (print_int(arg, 16, FALSE));
}
