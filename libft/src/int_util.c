/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   int_util.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgardner <stephenbgardner@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/09 18:23:34 by sgardner          #+#    #+#             */
/*   Updated: 2017/10/21 18:20:27 by sgardner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

intmax_t	get_int(t_arg *arg)
{
	intmax_t	n;

	if (F(F_H))
		n = (short)va_arg(*arg->ap, int);
	else if (F(F_HH))
		n = (char)va_arg(*arg->ap, int);
	else if (F(F_L))
		n = (long)va_arg(*arg->ap, long);
	else if (F(F_LL))
		n = (long long)va_arg(*arg->ap, long long);
	else if (F((F_J | F_Z)))
		n = va_arg(*arg->ap, intmax_t);
	else
		n = va_arg(*arg->ap, int);
	return (n);
}

uintmax_t	get_uint(t_arg *arg)
{
	uintmax_t	un;

	if (F(F_H))
		un = (unsigned short)va_arg(*arg->ap, unsigned int);
	else if (F(F_HH))
		un = (unsigned char)va_arg(*arg->ap, unsigned int);
	else if (F(F_L))
		un = (unsigned long)va_arg(*arg->ap, unsigned long);
	else if (F(F_LL))
		un = (unsigned long long)va_arg(*arg->ap, unsigned long long);
	else if (F(F_J))
		un = va_arg(*arg->ap, uintmax_t);
	else if (F(F_Z) || arg->conv == 'p')
		un = (size_t)va_arg(*arg->ap, size_t);
	else
		un = va_arg(*arg->ap, unsigned int);
	return (un);
}

char		*pf_itoa(t_arg *arg, intmax_t n, int base)
{
	uintmax_t	un;

	if (n < 0)
	{
		*arg->prefix = '-';
		un = (uintmax_t)(n * -1);
	}
	else
		un = (uintmax_t)n;
	return (pf_uitoa(un, base));
}

char		*pf_uitoa(uintmax_t un, int base)
{
	char	num[23];
	int		digit;

	num[22] = '\0';
	digit = 21;
	if (un == 0)
		num[digit--] = '0';
	while (un != 0)
	{
		num[digit--] = BASE_KEY[un % base];
		un /= base;
	}
	return (ft_strdup(&num[++digit]));
}
