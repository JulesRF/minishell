/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isprint.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfiszbin <vfiszbin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/29 13:22:38 by vfiszbin          #+#    #+#             */
/*   Updated: 2022/05/26 09:52:32 by vfiszbin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*Tests for any printing character, including space (‘ ’).  
(between 32 and 126 inclusive in ASCII table).
The value of the argument must be representable as an unsigned char or the
value of EOF*/
int	ft_isprint(int c)
{
	return (c >= 32 && c <= 126);
}
