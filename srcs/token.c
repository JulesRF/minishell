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

void	ft_parse_operator(t_token **token, t_list **bin, char c)
{
	if (c == '|')
		ft_lstadd_back_token(token, ft_lstnew_token(bin, "|", 1));//PROTECT
	if (c == '$')
		ft_lstadd_back_token(token, ft_lstnew_token(bin, "$", 1));//PROTECT
}

void	ft_parse_ponct(t_token **token, t_list **bin, char c)
{
	if (c == 39)
		ft_lstadd_back_token(token, ft_lstnew_token(bin, "\'", 3));//PROTECT
	if (c == 34)
		ft_lstadd_back_token(token, ft_lstnew_token(bin, "\"", 3));//PROTECT
}

int	ft_parse_redir(t_token **token, t_list **bin, char c, char *str)
{
	if (c == 60)
	{
		if (str[1] == 60)
		{
			ft_lstadd_back_token(token, ft_lstnew_token(bin, "<<", 5));//PROTECT
			return (1);
		}
		else
			ft_lstadd_back_token(token, ft_lstnew_token(bin, "<", 5));//PROTECT
	}
	if (c == 62)
	{
		if (str[1] == 62)
		{
			ft_lstadd_back_token(token, ft_lstnew_token(bin, ">>", 5));//PROTECT
			return (1);
		}
		else
			ft_lstadd_back_token(token, ft_lstnew_token(bin, ">", 5));//PROTECT
	}
	return (0);
}

int	ft_parse_word(t_token **token, t_list **bin, char *str)
{
	int	i;
	int	j;
	char *dest;

	i = 0;
	j = 0;
	while (str[i] != '|' && str[i] != '$' && str[i] != 39 && str[i] != 34
		&& str[i] != ' ' && str[i])
		i++;
	dest = malloc(sizeof(char) * i + 1);
	ft_lstadd_back(bin, ft_lstnew(dest));
	while (j < i)
	{
		dest[j] = str[j];
		j++;
	}
	dest[j] = '\0';
	ft_lstadd_back_token(token, ft_lstnew_token(bin, dest, 2));//PROTECT
	return (i - 1);
}

void	ft_token(t_token **token, t_list **bin, char *str)
{
	int	i;

	i = 0;
	while (str[i] != '\0')
	{
		if (str[i] == '|' || str[i] == '$')
			ft_parse_operator(token, bin, str[i]);
		else if (str[i] == 39 || str[i] == 34)
			ft_parse_ponct(token, bin, str[i]);
		else if (str[i] == 60 || str[i] == 62)
			i = i + ft_parse_redir(token, bin, str[i], str + i);
		else if (str[i] == ' ')
			ft_lstadd_back_token(token, ft_lstnew_token(bin, " ", 4));//PROTECT
		else
			i = i + ft_parse_word(token, bin, str + i);
		i++;
	}
}