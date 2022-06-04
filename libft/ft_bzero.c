/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_bzero.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfiszbin <vfiszbin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/29 16:09:57 by vfiszbin          #+#    #+#             */
/*   Updated: 2022/05/26 09:15:59 by vfiszbin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*Writes n zeroed bytes to the string s*/
void	ft_bzero(void *s, size_t n)
{
	ft_memset(s, '\0', n);
}
