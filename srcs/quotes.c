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

t_token	*ft_joincontent(t_token *temp, t_token *token, t_list **bin,
t_data *data)
{
	char	*str;
	int		i;
	int		j;

	if (temp == NULL)
		return (ft_lstnew_token(bin, data, token->content, token->type));
	str = malloc(sizeof(char) * (ft_strlen(temp->content)
		+ ft_strlen(token->content)) + 1);
	if (!str)

	ft_lstadd_backs(bin, ft_lstnew(str), data, bin);
	i = 0;
	j = 0;
	while (temp->content[i])
	{
		str[i] = temp->content[i];
		i++;
	}
	while (token->content[j])
		str[i++] = token->content[j++];
	str[i] = '\0';
	temp->content = str;
	return (temp);
}

void	ft_doublequotes(t_token *token, t_list **bin, t_data *data)//t_token *temp, t_token *stop)
{
	while (token)
	{
		if (!ft_strcmp(token->content, "\"") && token->type == 3)
		{
			data->stop = token->next;
			if (!ft_strcmp(data->stop->content, "\"") && data->stop->type == 3)
				token = ft_addempty(token, data->stop, bin, data);
			else
			{
				while (data->stop && ft_strcmp(data->stop->content, "\""))
				{
					if (data->stop->type != 6)
						data->temp = ft_joincontent(data->temp, data->stop, bin, data);
					data->stop = data->stop->next;
				}
				if (data->temp == NULL)
					data->temp = ft_lstnew_token(bin, data, "", 2);
				data->temp->next = data->stop;
				token->next = data->temp;
				ft_doublequotes(data->stop->next, bin, data);
				return ;
			}
		}
		token = token->next;
	}
}

void	ft_simplequotes(t_token *token, t_list **bin, t_data *data)//t_token *temp, t_token *stop)
{
	while (token)
	{
		if (!ft_strcmp(token->content, "\'") && token->type == 3)
		{
			data->stop = token->next;
			if (!ft_strcmp(data->stop->content, "\'") && data->stop->type == 3)
				token = ft_addempty(token, data->stop, bin, data);
			else
			{
				while (data->stop && ft_strcmp(data->stop->content, "\'"))
				{
					if (data->stop->type != 6)
						data->temp = ft_joincontent(data->temp, data->stop, bin, data);
					data->stop = data->stop->next;
				}
				if (data->temp == NULL)
					data->temp = ft_lstnew_token(bin, data, "", 2);
				data->temp->next = data->stop;
				token->next = data->temp;
				ft_simplequotes(data->stop->next, bin, data);
				return ;
			}
		}
		token = token->next;
	}
}
