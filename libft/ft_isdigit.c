/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isdigit.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfiszbin <vfiszbin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/29 11:38:51 by vfiszbin          #+#    #+#             */
/*   Updated: 2022/05/26 09:51:06 by vfiszbin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*Tests for a decimal digit character (between 0 and 9)*/
int	ft_isdigit(int c)
{
	return (c >= '0' && c <= '9');
}
