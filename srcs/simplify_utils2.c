/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simplify_utils2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jroux-fo <jroux-fo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/05 16:50:45 by jroux-fo          #+#    #+#             */
/*   Updated: 2022/07/05 16:50:48 by jroux-fo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_handleheredoc(t_token *token, t_list **bin, t_data *data)
{
	while (token)
	{
		if ((!ft_strcmp(token->content, "<<") && token->type == 5))
		{
			token = token->next;
			while (!ft_strcmp(token->content, " ") && token->type == 4)
				token = token->next;
			if (!ft_strcmp(token->content, "$") && token->type == 1)
			{
				if (!token->next)
					return ;
				token = ft_joincontent(token, token->next, bin, data);
				token->type = 2;
				token->next = token->next->next;
			}
		}
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

// void	ft_ghostbuster(t_token **token, t_list **bin, t_data *data)
// {
// 	t_token *tmp;
// 	t_token *stop;

// 	tmp = *token;
// 	while (tmp)
// 	{
// 		if (tmp->type != 5)
// 		{
// 			stop = tmp;
// 			if (stop->next && stop->next->type == 6)
// 			{
// 				printf("la on delete celui la ->%s<-\n", stop->next->content);
// 				ft_delete_token(token, stop->next);
// 				stop = stop->next;
// 				if (stop->type == 2 && stop->next && stop->next->type == 2)
// 				{
// 					stop = ft_joincontent(stop, stop->next, bin, data);
// 					stop->next = stop->next->next;
// 				}
// 			}
// 			else
// 				tmp = tmp->next;
// 		}
// 		else
// 			tmp = tmp->next;
// 	}
// }
