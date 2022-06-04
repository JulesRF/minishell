/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfiszbin <vfiszbin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/16 10:22:15 by vfiszbin          #+#    #+#             */
/*   Updated: 2022/06/01 18:26:13 by vfiszbin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	parse_formats(const char *fmt, va_list *ap, int *i, int *nb_char)
{
	if (fmt[*i + 1] == 'd' || fmt[*i + 1] == 'i')
		ft_printnbr_fd(va_arg(*ap, int), 1, nb_char);
	else if (fmt[*i + 1] == 's')
		ft_printstr_fd(va_arg(*ap, char *), 1, nb_char);
	else if (fmt[*i + 1] == 'c')
		ft_printchar_fd(va_arg(*ap, int), 1, nb_char);
	else if (fmt[*i + 1] == 'p')
	{
		ft_printstr_fd("0x", 1, nb_char);
		ft_printptr_base(va_arg(*ap, unsigned long long), HEXA, nb_char);
	}
	else if (fmt[*i + 1] == 'u')
		ft_print_uint_fd(va_arg(*ap, unsigned int), 1, nb_char);
	else if (fmt[*i + 1] == 'x')
		ft_printnbr_base(va_arg(*ap, unsigned int), HEXA, nb_char);
	else if (fmt[*i + 1] == 'X')
		ft_printnbr_base(va_arg(*ap, unsigned int), HEXAMAJ, nb_char);
	else if (fmt[*i + 1] == '%')
		ft_printchar_fd('%', 1, nb_char);
	else
		return (0);
	*i = *i + 1;
	return (1);
}

int	ft_printf(const char *fmt, ...)
{
	va_list	ap;
	int		i;
	int		nb_char;

	va_start(ap, fmt);
	i = 0;
	nb_char = 0;
	while (fmt[i])
	{
		if (fmt[i] == '%' && fmt[i + 1])
		{
			if (! parse_formats(fmt, &ap, &i, &nb_char))
				break ;
		}
		else
			ft_printchar_fd(fmt[i], 1, &nb_char);
		i++;
	}
	va_end(ap);
	return (nb_char);
}
