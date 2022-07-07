/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jroux-fo <jroux-fo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/22 14:54:56 by jroux-fo          #+#    #+#             */
/*   Updated: 2022/06/22 14:54:56 by jroux-fo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_parse_operator(t_token **token, t_list **bin, t_data *data, int i)
{
	if (data->cmd_line[i] == '|')
		ft_lstadd_back_token(token, ft_lstnew_token(bin, data, "|", 1),
			data);
	if (data->cmd_line[i] == '$')
		ft_lstadd_back_token(token, ft_lstnew_token(bin, data, "$", 1),
			data);
}

void	ft_parse_ponct(t_token **token, t_list **bin, t_data *data, int i)
{
	if (data->cmd_line[i] == 39)
		ft_lstadd_back_token(token, ft_lstnew_token(bin, data, "\'", 3),
			data);
	if (data->cmd_line[i] == 34)
		ft_lstadd_back_token(token, ft_lstnew_token(bin, data, "\"", 3),
			data);
}

int	ft_parse_redir(t_token **token, t_list **bin, t_data *data, int i)
{
	if (data->cmd_line[i] == '<')
	{
		if (data->cmd_line[i + 1] == '<')
		{
			ft_lstadd_back_token(token, ft_lstnew_token(bin, data, "<<", 5),
				data);
			return (1);
		}
		else
			ft_lstadd_back_token(token, ft_lstnew_token(bin, data, "<", 5),
				data);
	}
	if (data->cmd_line[i] == '>')
	{
		if (data->cmd_line[i + 1] == '>')
		{
			ft_lstadd_back_token(token, ft_lstnew_token(bin, data, ">>", 5),
				data);
			return (1);
		}
		else
			ft_lstadd_back_token(token, ft_lstnew_token(bin, data, ">", 5),
				data);
	}
	return (0);
}

int	ft_parse_word(t_token **token, t_list **bin, t_data *data, char *str)
{
	int		i;
	int		j;
	char	*dest;

	i = 0;
	j = 0;
	while (str[i] != '|' && str[i] != '$' && str[i] != '\'' && str[i] != '\"'
		&& str[i] != ' ' && str[i] != '<' && str[i] != '>' && str[i])
		i++;
	dest = malloc(sizeof(char) * i + 1);
	ft_lstadd_backs(bin, ft_lstnew(dest), data);
	while (j < i)
	{
		dest[j] = str[j];
		j++;
	}
	dest[j] = '\0';
	ft_lstadd_back_token(token, ft_lstnew_token(bin, data, dest, 2), data);
	return (i - 1);
}

void	ft_token(t_token **token, t_list **bin, t_data *data)
{
	int	i;

	i = 0;
	while (data->cmd_line[i] != '\0')
	{
		if (data->cmd_line[i] == '|' || data->cmd_line[i] == '$')
			ft_parse_operator(token, bin, data, i);
		else if (data->cmd_line[i] == '\'' || data->cmd_line[i] == '\"')
			ft_parse_ponct(token, bin, data, i);
		else if (data->cmd_line[i] == '<' || data->cmd_line[i] == '>')
			i = i + ft_parse_redir(token, bin, data, i);
		else if (data->cmd_line[i] == ' ')
			ft_lstadd_back_token(token, ft_lstnew_token(bin, data, " ", 4),
				data);
		else
			i = i + ft_parse_word(token, bin, data, data->cmd_line + i);
		i++;
	}
}
