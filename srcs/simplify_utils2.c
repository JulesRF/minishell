/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simplify_utils2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jroux-fo <jroux-fo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/05 16:50:45 by jroux-fo          #+#    #+#             */
/*   Updated: 2022/07/10 22:44:12 by jroux-fo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_handleheredoc(t_token *tken, t_list **bin, t_data *data)
{
	while (tken)
	{
		if (tken && (!ft_strcmp(tken->content, "<<") && tken->type == 5))
		{
			tken = tken->next;
			while (tken && !ft_strcmp(tken->content, " ") && tken->type == 4)
				tken = tken->next;
			if (tken && !ft_strcmp(tken->content, "$") && tken->type == 1)
			{
				if (!tken->next)
					return ;
				tken = ft_joincontent(tken, tken->next, bin, data);
				tken->type = 2;
				tken->next = tken->next->next;
			}
		}
		if (tken)
			tken = tken->next;
	}
}

int	ft_add(t_token *token)
{
	return (token->type + token->qt);
}

t_token	*ft_skipspace(t_token *token)
{
	while (token && token->type == 4)
		token = token->next;
	return (token);
}

// void	ft_quotesbool(t_token *token)
// {
// 	t_token	*temp;
// 	t_token	*temp2;
// 	t_token	*stop;

// 	while (token)
// 	{
// 		stop = ft_skipspace(token);
// 		if (!stop)
// 			return ;
// 		temp = stop;
// 		stop = stop->next;
// 		stop = ft_skipspace(stop);
// 		if (!stop)
// 			return ;
// 		if (stop->type == 3)
// 		{
// 			temp2 = ft_skipspace(stop->next);
// 			if (!temp2)
// 				return ;
// 			temp->qt = 10;
// 			temp2->qt = 10;
// 		}
// 		token = token->next;
// 	}
// }

void	ft_quotesbool(t_token *token)
{
	// t_token *temp2;
	t_token	*temp;

	while (token)
	{
		temp = token;
		while(temp && temp->type == 4)
			temp = temp->next;
		if (temp->type == 3)
		{
			temp = temp->next;
			if (!temp)
				return ;
			while (temp && temp->type != 3)
			{
				temp->qt = 10;
				temp = temp->next;
			}
			temp = temp->next;
			if (!temp)
				return ;
			while (temp && temp->type == 4)
				temp = temp->next;
			if (!temp)
				return ;
			temp->qt = 10;
			token = temp;
		}
		else
			token = token->next;
	}
}

void	ft_ghostbuster(t_token **token, t_list **bin, t_data *data)
{
	t_token	*tmp;

	(void)bin;
	(void)data;
	tmp = *token;
	while (tmp)
	{
		if (tmp->type != 5 && tmp->type != 4)
		{
			tmp = tmp->next;
			while (tmp && tmp->type == 4)
				tmp = tmp->next;
			if (tmp && tmp->type == 6)
				ft_delete_token(token, tmp);
		}
		else
			tmp = tmp->next;
	}
}
