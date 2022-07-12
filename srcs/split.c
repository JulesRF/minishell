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

// t_token *ft_splitspaces(t_token *token, t_list **bin, t_data *data)
// {
// 	int		i;
// 	char	*str;

// 	i = 0;
// 	if (token && token->qt != 0)
// 		return (token->next);
// 	printf("TOKEN CONTENT ->%s<-\n", token->content);
// 	if (token && token->type == 2 && (ft_isspace(token->content) != -5))
// 	{
// 		printf("OEEEEEEEEEEEEEEEEEEEEEEEEEEEEE TOKEN->%s<-\n", token->content);
// 		while (token->content[i] && token->content[i] != ' ')
// 			i++;
// 		str = token->content;
// 		str[i] = '\0';
// 		while (token->content[i] && token->content[i] == ' ')
// 			i++;
// 		data->temp = ft_lstnew_token(bin, data, token->content + i, 2);
// 		token->content = str;
// 		data->temp->next = token->next;
// 		token->next = data->temp;
// 		return (data->temp);
// 	}
// 	return (token->next);
// }

// void	ft_splitres(t_token *token, t_list **bin, t_data *data)
// {
// 	while (token)
// 		token = ft_splitspaces(token, bin, data);
// }

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

void	ft_splitres(t_token *token, t_list **bin, t_data *data)
{
	char	*str;
	int		i;
	int		j;

	i = 0;
	j = 0;
	while (token)
	{
		if (token && token->qt == 0 && token->type == 2 && ft_isspace(token->content))
		{
			j = ft_skip_space(token->content);
			while (token->content[i + j] && token->content[i + j] != ' ')
				i++;
			str = token->content + j;
			str[i] = '\0';
			i++;
			while (token->content[i + j] && token->content[i + j] == ' ')
				i++;
			data->temp = ft_lstnew_token(bin, data, token->content + i, 2);
			token->content = str;
			data->temp->next = token->next;
			token->next = data->temp;
		}
		token = token->next;
	}
}