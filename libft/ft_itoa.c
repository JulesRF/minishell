/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfiszbin <vfiszbin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/06 15:54:18 by vfiszbin          #+#    #+#             */
/*   Updated: 2022/05/26 09:22:49 by vfiszbin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	ft_nbr_len(int n, int *len)
{
	while (n > 0)
	{
		*len = *len + 1;
		n /= 10;
	}
}

static char	*ft_neg_itoa(int n)
{
	int		len;
	char	*ret_str;

	n *= -1;
	len = 1;
	ft_nbr_len(n, &len);
	ret_str = malloc(sizeof(char) * (len + 1));
	if (!ret_str)
		return (NULL);
	ret_str[len] = '\0';
	len--;
	while (n > 0)
	{
		ret_str[len] = n % 10 + 48;
		n /= 10;
		len--;
	}
	ret_str[len] = '-';
	return (ret_str);
}

static char	*ft_zero_itoa(void)
{
	char	*ret_str;

	ret_str = malloc(sizeof(char) * (2));
	ret_str[0] = '0';
	ret_str[1] = '\0';
	return (ret_str);
}

/*Allocates (with malloc) and returns a string
representing the integer received as an argument.
Negative numbers must be handled*/
char	*ft_itoa(int n)
{
	int		len;
	char	*ret_str;

	if (n == -2147483648)
		return (ft_strdup("-2147483648"));
	if (n == 0)
		return (ft_zero_itoa());
	if (n < 0)
		return (ft_neg_itoa(n));
	len = 0;
	ft_nbr_len(n, &len);
	ret_str = malloc(sizeof(char) * (len + 1));
	if (!ret_str)
		return (NULL);
	ret_str[len] = '\0';
	len--;
	while (n > 0)
	{
		ret_str[len] = n % 10 + 48;
		n /= 10;
		len--;
	}
	return (ret_str);
}
