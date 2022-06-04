/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printstr_fd.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfiszbin <vfiszbin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/18 11:28:57 by vfiszbin          #+#    #+#             */
/*   Updated: 2022/05/30 19:14:52 by vfiszbin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_printstr_fd(char *s, int fd, int *nb_char)
{
	int	i;

	if (!s)
	{
		ft_printstr_fd("(null)", 1, nb_char);
		return ;
	}
	i = 0;
	while (s[i])
	{
		ft_printchar_fd(s[i], fd, nb_char);
		i++;
	}
}
