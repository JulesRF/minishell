/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   garbage.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jroux-fo <jroux-fo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/22 14:37:57 by jroux-fo          #+#    #+#             */
/*   Updated: 2022/06/22 14:37:57 by jroux-fo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_garbage(t_list **bin)
{
	t_list	*to_suppr;

	while (*bin)
	{
		to_suppr = *bin;
		*bin = to_suppr->next;
		free(to_suppr->content);
		free(to_suppr);
	}
	*bin = NULL;
}

t_list	*ft_lstnew(void *content)
{
	t_list	*newcell;

	newcell = malloc(sizeof(t_list));
	if (!newcell)
		return (NULL);
	newcell->content = content;
	newcell->next = NULL;
	return (newcell);
}

t_list	*ft_lstlast(t_list *lst)
{
	if (!lst)
		return (NULL);
	while (lst->next)
		lst = lst->next;
	return (lst);
}

void	ft_lstadd_backs(t_list **alst, t_list *new, t_data *data, t_list **bin)
{
	t_list	*temp;

	if (!new)
	{
		clean_prog(data->env, bin, data->cmd_line);
		exit_prog("exit\nminishell: exit: bas malloc allocation", 2);
	}
	if (!*alst)
	{
		*alst = new;
		return ;
	}
	temp = ft_lstlast(*alst);
	temp->next = new;
}
