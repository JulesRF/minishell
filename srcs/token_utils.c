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

t_token	*ft_lstnew_token(t_list **bin, t_data *data, char *content, int type)
{
	t_token	*newcell;

	newcell = malloc(sizeof(t_token));
	if (!newcell)
		return (NULL);
	ft_lstadd_backs(bin, ft_lstnew(newcell), data, bin);
	if (type > 10)
	{
		newcell->qt = 10;
		type = type - 10;
	}
	else
		newcell->qt = 0;
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

void	ft_lstadd_back_token(t_token **alst, t_token *new, t_data *data,
t_list **bin)
{
	t_token	*temp;

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
		return ;
	}
	while (tmp && tmp != to_del)
	{
		prev = tmp;
		tmp = tmp->next;
	}
	if (tmp == NULL)
		return ;
	prev->next = tmp->next;
}

void	ft_clean_token(t_token **token)
{
	*token = NULL;
}
