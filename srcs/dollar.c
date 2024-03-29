/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dollar.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jroux-fo <jroux-fo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/22 15:01:28 by jroux-fo          #+#    #+#             */
/*   Updated: 2022/06/22 15:01:28 by jroux-fo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_dollarfind(t_token *token, char *to_find, t_data *data, t_list **bin)
{
	int		i;
	char	**env;

	env = *data->env;
	i = 0;
	if (ft_dollarcheck(token, to_find, env, bin))
		return ;
	while (env[i])
	{
		if ((!ft_strncmp(env[i], to_find, ft_strlen(to_find)))
			&& (env[i][ft_strlen(to_find)] == '='))
		{
			token->content = ft_strdup((env[i] + (ft_strlen(to_find) + 1)));
			ft_lstadd_backs(bin, ft_lstnew(token->content), data);
			token->type = 2;
			return ;
		}
		i++;
	}
	token->type = 6;
	token->content = ft_strjoin(token->content, to_find);
	ft_lstadd_backs(bin, ft_lstnew(token->content), data);
}

t_token	*ft_isdollar(t_token *token, t_list **bin, t_data *data, int inf)
{
	if (!ft_strcmp(token->content, "$") && token->type == 1)
	{
		if (token->next && (inf == 1 || token->next->content[0] == '='))
		{
			token->type = 2;
			return (token->next);
		}
		if (!token->next || token->next->type != 2)
		{
			token->type = 2;
			return (token->next);
		}
		if (!ft_strcmp(token->next->content, "?"))
		{
			token->content = ft_itoa(g_exit_status);
			if (!token->content)
				return (NULL);
			ft_lstadd_backs(bin, ft_lstnew(token->content), data);
			token->type = 2;
		}
		else
			ft_dollarfind(token, token->next->content, data, bin);
		token->next = token->next->next;
	}
	return (token->next);
}

void	ft_dollar(t_token *token, t_list **bin, t_data *data)
{
	while (token)
	{
		if (!ft_strcmp(token->content, "\'") && token->type == 3)
		{
			token = token->next;
			if (!token)
				return ;
			while (token && (ft_strcmp(token->content, "\'")
					|| token->type != 3))
				token = ft_isdollar(token, bin, data, 1);
		}
		if (!token)
			return ;
		if (!ft_strcmp(token->content, "\"") && token->type == 3)
		{
			token = token->next;
			if (!token)
				return ;
			while (token && (ft_strcmp(token->content, "\"")
					|| token->type != 3))
				token = ft_isdollar(token, bin, data, 0);
		}
		token = ft_isdollar(token, bin, data, 0);
	}
}

t_token	*ft_splitdollar(t_token *token, t_list **bin, int i, t_data *data)
{
	char	*str;
	t_token	*temp;
	t_token	*stop;

	if (token->next && !ft_strcmp(token->content, "$") && token->type == 1)
	{
		if (!ft_strcmp(token->next->content, "?") || token->next->type != 2)
			return (token->next);
		stop = token->next;
		while (stop->content[i])
		{
			if (stop->content[i] && !ft_isalnum(stop->content[i]))
			{
				str = stop->content;
				ft_fucknorm(stop, str + i, bin, data);
				str[i] = '\0';
				temp = ft_lstnew_token(bin, data, str, 2);
				temp->next = stop;
				token->next = temp;
				return (temp);
			}
			i++;
		}
	}
	return (token->next);
}

void	ft_sepdollar(t_token *token, t_list **bin, t_data *data)
{
	while (token)
	{
		if (!ft_strcmp(token->content, "\'") && token->type == 3)
		{
			token = token->next;
			if (!token)
				return ;
			while (token && (ft_strcmp(token->content, "\'")
					|| token->type != 3))
				token = token->next;
		}
		if (!token)
			return ;
		if (!ft_strcmp(token->content, "\"") && token->type == 3)
		{
			token = token->next;
			if (!token)
				return ;
			while (token && (ft_strcmp(token->content, "\"")
					|| token->type != 3))
				token = ft_splitdollar(token, bin, 0, data);
		}
		token = ft_splitdollar(token, bin, 0, data);
	}
}
