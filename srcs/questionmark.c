/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   questionmark.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jroux-fo <jroux-fo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/28 15:24:32 by jroux-fo          #+#    #+#             */
/*   Updated: 2022/06/28 15:24:34 by jroux-fo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_is_questionmark(char *str)
{
	if (!str)
		return (0);
	if (str[0] == '?')
		return (1);
	return (0);
}

void	ft_questionmark(t_token *token, t_list **bin, t_data *data)
{
	t_token	*stop;
	t_token	*temp;

	while (token)
	{
		if (!token->next)
			return ;
		if (!ft_strcmp(token->content, "$") && token->type == 1)
		{
			stop = token->next;
			if (ft_is_questionmark(token->next->content))
			{
				temp = ft_lstnew_token(bin, data, stop->content + 1, 2);
				stop->content = "?";
				temp->next = stop->next;
				stop->next = temp;
				token->next = stop;
				return ;
			}
		}
		token = token->next;
	}
}
