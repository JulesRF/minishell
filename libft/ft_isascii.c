/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isascii.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfiszbin <vfiszbin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/29 12:22:32 by vfiszbin          #+#    #+#             */
/*   Updated: 2022/05/26 09:50:24 by vfiszbin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*Tests for an ASCII character, which is any character between 0 and 127
inclusive*/
int	ft_isascii(int c)
{
	return (c >= 0 && c <= 127);
}
