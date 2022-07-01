/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jroux-fo <jroux-fo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/22 14:57:46 by jroux-fo          #+#    #+#             */
/*   Updated: 2022/06/22 14:57:46 by jroux-fo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token	*ft_joincontent(t_token *temp, t_token *token, t_list **bin)
{
	char	*str;
	int		i;
	int		j;

	if (temp == NULL)
		return (ft_lstnew_token(bin, token->content, token->type));
	str = malloc(sizeof(char) * (ft_strlen(temp->content)
		+ ft_strlen(token->content)) + 1);
	ft_lstadd_back(bin, ft_lstnew(str));
	i = 0;
	j = 0;
	while (temp->content[i])
	{
		str[i] = temp->content[i];
		i++;
	}
	while (token->content[j])
	{
		str[i] = token->content[j];
		i++;
		j++;
	}
	str[i] = '\0';
	temp->content = str;
	return (temp);
}

void	ft_doublequotes(t_token *token, t_list **bin, t_token *temp,
t_token *stop)
{
	while (token)
	{
		if (!ft_strcmp(token->content, "\"") && token->type == 3)
		{
			stop = token->next;
			if (!ft_strcmp(stop->content, "\"") && stop->type == 3)
				token = ft_addempty(token, stop, bin);
			else
			{
				while (stop && ft_strcmp(stop->content, "\""))
				{
					if (stop->type != 6)
						temp = ft_joincontent(temp, stop, bin);
					stop = stop->next;
				}
				if (temp == NULL)
					temp = ft_lstnew_token(bin, "", 2);
				temp->next = stop;
				token->next = temp;
				ft_doublequotes(stop->next, bin, temp, stop);
				return ;
			}
		}
		token = token->next;
	}
}

void	ft_simplequotes(t_token *token, t_list **bin, t_token *temp,
t_token *stop)
{
	while (token)
	{
		if (!ft_strcmp(token->content, "\'") && token->type == 3)
		{
			stop = token->next;
			if (!ft_strcmp(stop->content, "\'") && stop->type == 3)
				token = ft_addempty(token, stop, bin);
			else
			{
				while (stop && ft_strcmp(stop->content, "\'"))
				{
					if (stop->type != 6)
						temp = ft_joincontent(temp, stop, bin);
					stop = stop->next;
				}
				if (temp == NULL)
					temp = ft_lstnew_token(bin, "", 2);
				temp->next = stop;
				token->next = temp;
				ft_simplequotes(stop->next, bin, temp, stop);
				return ;
			}
		}
		token = token->next;
	}
}
