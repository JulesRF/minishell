/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_uint_fd.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfiszbin <vfiszbin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/17 12:24:55 by vfiszbin          #+#    #+#             */
/*   Updated: 2022/05/30 19:14:55 by vfiszbin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_print_uint_fd(unsigned int n, int fd, int *nb_char)
{
	if (n >= 10)
		ft_printnbr_fd(n / 10, fd, nb_char);
	ft_printchar_fd((n % 10) + '0', fd, nb_char);
}
