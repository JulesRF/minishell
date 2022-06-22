/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jroux-fo <jroux-fo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/22 14:40:54 by jroux-fo          #+#    #+#             */
/*   Updated: 2022/06/22 14:40:54 by jroux-fo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token	*ft_lstnew_token(t_list **bin, char *content, int type)
{
	t_token	*newcell;

	newcell = malloc(sizeof(t_token));
	ft_lstadd_back(bin, ft_lstnew(newcell));
	if (!newcell)
		return (NULL);
	newcell->content = content;
	newcell->type = type;
	newcell->next = NULL;
	return (newcell);
}

t_token	*ft_lstlast_token(t_token *lst)
{
	if (!lst)
		return (NULL);
	while (lst->next)
		lst = lst->next;
	return (lst);
}

void	ft_lstadd_back_token(t_token **alst, t_token *new)
{
	t_token	*temp;

	if (!*alst)
	{
		*alst = new;
		return ;
	}
	temp = ft_lstlast_token(*alst);
	temp->next = new;
}

void	ft_delete_token(t_token **alst, t_token *to_del)
{
	t_token	*tmp;
	t_token	*prev;

	tmp = *alst;
	if (tmp == to_del)
	{
		*alst = tmp->next;
		return ;//free deleted token ?
	}
	while (tmp && tmp != to_del)
	{
		prev = tmp;
		tmp = tmp->next;
	}
	if (tmp == NULL)
		return ;
	prev->next = tmp->next;
	//free deleted token ?
}

void	ft_clean_token(t_token **token)
{
	*token = NULL;
}
