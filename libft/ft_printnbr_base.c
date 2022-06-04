/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printnbr_base.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfiszbin <vfiszbin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/18 09:32:30 by vfiszbin          #+#    #+#             */
/*   Updated: 2022/05/30 19:14:49 by vfiszbin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	check_init_error(char *base)
{
	int	i;
	int	j;

	if (base[0] == 0 || base[1] == 0)
		return (1);
	i = 0;
	j = 0;
	while (base[i] != '\0')
	{
		j = i;
		if (base[i] == '+' || base[i] == '-'
			|| base[i] <= 32 || base[i] > 126)
			return (1);
		while (base[j] != '\0')
		{
			j++;
			if (base[j] == base[i])
				return (1);
		}
		i++;
	}
	return (0);
}

void	ft_printnbr_base(unsigned int nbr, char *base, int *nb_char)
{
	unsigned int	base_len;
	unsigned int	num;

	num = nbr;
	if (check_init_error(base) == 0)
	{
		base_len = 0;
		while (base[base_len])
			base_len++;
		if (num >= base_len)
		{
			ft_printnbr_base(num / base_len, base, nb_char);
			ft_printnbr_base(num % base_len, base, nb_char);
		}
		if (num < base_len)
			ft_printchar_fd(base[num], 1, nb_char);
	}
}
