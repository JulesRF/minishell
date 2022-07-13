/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jroux-fo <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/11 18:50:10 by jroux-fo          #+#    #+#             */
/*   Updated: 2022/07/11 18:50:12 by jroux-fo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_isspace(char *str)
{
	int i;

	i = 0;
	while (str[i])
	{
		if (str[i] == ' ')
			return (1);
		i++;
	}
	return (0);
}

int	ft_skip_space(char *str)
{
	int	i;

	i = 0;
	if (str[i] == ' ')
	{
		while (str[i] == ' ')
			i++;
	}
	return (i);
}

void	ft_splitsuite(t_token *token, t_list **bin, t_data *data)
{
	char	*str;

	data->j = ft_skip_space(token->content);
	while (token->content[data->i + data->j] && token->content[data->i + data->j] != ' ')
		data->i++;
	str = token->content + data->j;
	str[data->i] = '\0';
	data->i++;
	while (token->content[data->i + data->j] && token->content[data->i + data->j] == ' ')
		data->i++;
	data->temp = ft_lstnew_token(bin, data, token->content + data->i + data->j, 2);
	if (!data->temp->content[0] == '\0')
		data->temp = NULL;
	else
		data->temp->next = token->next;
	token->content = str;
	token->next = data->temp;
}

void	ft_splitres(t_token *token, t_list **bin, t_data *data)
{
	while (token)
	{
		data->i = 0;
		data->j = 0;
		if (token && token->qt == 0 && token->type == 2 && ft_isspace(token->content)
			&& (ft_isspace(token->content) != (int)ft_strlen(token->content)))
		{
			ft_splitsuite(token, bin, data);
			// data->j = ft_skip_space(token->content);
			// while (token->content[data->i + data->j] && token->content[i + j] != ' ')
			// 	i++;
			// str = token->content + data->j;
			// str[i] = '\0';
			// i++;
			// while (token->content[i + j] && token->content[i + j] == ' ')
			// 	i++;
			// data->temp = ft_lstnew_token(bin, data, token->content + i + j, 2);
			// if (!data->temp->content[0] == '\0')
			// 	data->temp = NULL;
			// else
			// 	data->temp->next = token->next;
			// token->content = str;
			// token->next = data->temp;
		}
		token = token->next;
	}
}