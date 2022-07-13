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

	data->i = 0;
	data->j = 0;
	if (temp == NULL)
	{
		if (token->type != 2 || token->type != 6)
			return (ft_lstnew_token(bin, data, token->content, 2 + token->qt));
		return (ft_lstnew_token(bin, data, token->content, ft_add(token)));
	}
	str = malloc(sizeof(char) * (ft_strlen(temp->content)
				+ ft_strlen(token->content)) + 1);
	if (!str)
		ft_getmeout(data);
	ft_lstadd_backs(bin, ft_lstnew(str), data);
	while (temp->content[data->i])
	{
		str[data->i] = temp->content[data->i];
		data->i++;
	}
	while (token->content[data->j])
		str[data->i++] = token->content[data->j++];
	str[data->i] = '\0';
	temp->content = str;
	return (temp);
}

void	ft_doublequotes(t_token *token, t_list **bin, t_data *data,
t_token *temp)
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
				while (data->stop && !ft_is_quotes(data->stop))
				{
					if (data->stop->type != 6)
						temp = ft_joincontent(temp, data->stop, bin, data);
					data->stop = data->stop->next;
				}
				if (temp == NULL)
					temp = ft_lstnew_token(bin, data, "", 2);
				temp->next = data->stop;
				token->next = temp;
				ft_doublequotes(data->stop->next, bin, data, NULL);
				return ;
			}
		}
		token = token->next;
	}
}

void	ft_simplequotes(t_token *token, t_list **bin, t_data *data,
t_token *temp)
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
						temp = ft_joincontent(temp, data->stop, bin, data);
					data->stop = data->stop->next;
				}
				if (temp == NULL)
					temp = ft_lstnew_token(bin, data, "", 2);
				temp->next = data->stop;
				token->next = temp;
				ft_simplequotes(data->stop->next, bin, data, NULL);
				return ;
			}
		}
		token = token->next;
	}
}

t_token	*ft_quotessuite(t_token *token, t_data *data)
{
	token->qt = 10;
	data->temp = data->temp->next;
	if (!data->temp)
		return (NULL);
	while (data->temp && data->temp->type != 3)
	{
		data->temp->qt = 10;
		data->temp = data->temp->next;
	}
	if (!data->temp)
		return (NULL);
	data->temp = data->temp->next;
	while (data->temp && data->temp->type == 4)
		data->temp = data->temp->next;
	if (!data->temp)
		return (NULL);
	data->temp->qt = 10;
	return (data->temp);
}

void	ft_quotesbool(t_token *token, t_data *data)
{
	while (token)
	{
		data->temp = token->next;
		if (token && token->type == 3)
			data->temp = token;
		while (data->temp && data->temp->type == 4)
			data->temp = data->temp->next;
		if (data->temp && data->temp->type == 3)
		{
			token = ft_quotessuite(token, data);
			if (!token)
				return ;
		}
		else
			token = token->next;
	}
}
