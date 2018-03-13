/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   float_util.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgardner <stephenbgardner@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/21 11:46:05 by sgardner          #+#    #+#             */
/*   Updated: 2017/10/21 19:55:24 by sgardner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static void	round_fstring(char *num, int base)
{
	char	*end;

	end = num;
	if (*num >= BASE_KEY[base / 2])
	{
		num--;
		while (*num == BASE_KEY[base - 1] || *num == '.')
		{
			if (*num != '.')
				*num = '0';
			num--;
		}
		*num = BASE_KEY[(*num - '0') + 1];
	}
	*end = '\0';
}

static void	load_mantissa(char *num, long double f, int precision, float base)
{
	if (precision == -1)
	{
		*num = BASE_KEY[(int)(f * base)];
		round_fstring(num, (int)base);
	}
	else
	{
		*num++ = '.';
		precision++;
		while (precision--)
		{
			f *= base;
			*num++ = BASE_KEY[(int)f];
			f -= (int)f;
		}
		*num = '\0';
		round_fstring(num - 1, (int)base);
	}
}

long double	get_float(t_arg *arg)
{
	long double	f;

	f = (F(F_LD)) ? va_arg(*arg->ap, long double) : va_arg(*arg->ap, double);
	if (f == 1.0 / 0.0 || f == -1.0 / 0.0 || f != f)
		arg->flags |= F_SPECIAL;
	if (f < 0.0)
	{
		*arg->prefix = '-';
		f *= -1.0;
	}
	return (f);
}

char		*pf_ftoa(long double f, int precision, float base)
{
	char		*num;
	long double	power;
	int			len;
	int			digit;
	int			i;

	power = 1.0;
	len = 2;
	while (power < f / base && len++)
		power *= base;
	num = (char *)malloc(len + (precision > 0) + precision + 1);
	i = 0;
	while (power > 0.0)
	{
		digit = (int)(f / power);
		num[i++] = BASE_KEY[digit];
		f -= digit * power;
		power = (power != 1.0) ? power / base : 0.0;
	}
	if (precision == 0)
		precision = -1;
	load_mantissa(&num[i], f, precision, base);
	return (num);
}
