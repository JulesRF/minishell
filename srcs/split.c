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

	if (!str)
		return (-5);
	while (str[i])
	{
		if (str[i] == ' ')
			return (i);
		i++;
	}
	return (-5);
}

t_token *ft_splitspaces(t_token *token, t_list **bin, t_data *data)
{
	int		i;
	char	*str;

	i = 0;
	if (token && token->qt != 0)
		return (token->next);
	if (token && token->type == 2)
	{
		while (token->content[i] && token->content[i] != ' ')
			i++;
		str = token->content;
		str[i] = '\0';
		while (token->content[i] && token->content[i] == ' ')
			i++;
		data->temp = ft_lstnew_token(bin, data, token->content + i, 2);
		token->content = str;
		data->temp->next = token->next;
		token->next = data->temp;
		return (data->temp);
	}
	return (token->next);
}

void	ft_splitres(t_token *token, t_list **bin, t_data *data)
{
	while (token)
		token = ft_splitspaces(token, bin, data);
}