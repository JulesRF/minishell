/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfiszbin <vfiszbin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/05 19:32:29 by vfiszbin          #+#    #+#             */
/*   Updated: 2022/05/26 09:18:13 by vfiszbin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
/*Contiguously allocates enough space for nmemb objects that are size 
bytes of memory each and returns a pointer to the allocated
memory. The allocated memory is filled with bytes of value zero*/
void	*ft_calloc(size_t nmemb, size_t size)
{
	void	*p;
	int		len;

	p = NULL;
	len = size * nmemb;
	p = malloc(len);
	if (!p)
		return (NULL);
	ft_memset(p, 0, len);
	return (p);
}
