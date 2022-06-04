/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstnew.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfiszbin <vfiszbin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/25 19:34:55 by vfiszbin          #+#    #+#             */
/*   Updated: 2022/05/26 09:55:29 by vfiszbin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*Allocates (with malloc) and returns a new node.
The member variable ’content’ is initialized with
the value of the parameter ’content’.  The variable
’next’ is initialized to NULL*/
t_list	*ft_lstnew(void *content)
{
	t_list	*new_el;

	new_el = malloc(sizeof(t_list));
	if (!new_el)
		return (NULL);
	new_el->content = content;
	new_el->next = NULL;
	return (new_el);
}
