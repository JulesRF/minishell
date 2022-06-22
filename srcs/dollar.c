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

char	*ft_dollarfind(char *to_find, char **env, t_list **bin)
{
	(void)bin;
	int		i;

	i = 0;
	if (!ft_strcmp(to_find, " "))
		return ("$ ");
	while (env[i])
	{
		if ((!ft_strncmp(env[i], to_find, ft_strlen(to_find)))
			&& (env[i][ft_strlen(to_find)] == '='))
			return (env[i] + (ft_strlen(to_find) + 1));
		i++;
	}
	return ("");
}


t_token	*ft_isdollar(t_token *token, t_list **bin, char **env)
{
	(void)bin;
	if (!ft_strcmp(token->content, "$") && token->type == 1)
	{
		if (!token->next || token->next->type != 2)
		{
			token->type = 2;
			return (token->next);
		}
		if (ft_strcmp(token->next->content, "?") == 0)
		{
			token->content = ft_itoa(g_exit_status);
			if (!token->content)
					return NULL; 
			ft_lstadd_back(bin, ft_lstnew(token->content));//PROTECT
		}
		else
			token->content = ft_dollarfind(token->next->content, env, bin);

		token->type = 2;
		token->next = token->next->next;
	}
	return (token->next);
}

void	ft_dollar(t_token *token, t_list **bin, char **env)
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
				token = ft_isdollar(token, bin, env);//PROTECT
		}
		token = ft_isdollar(token, bin, env);//PROTECT
	}
}

t_token	*ft_splitdollar(t_token *token, t_list **bin, int i, t_token *stop)
{
	char	*str;
	t_token *temp;

	if (!ft_strcmp(token->content, "$") && token->type == 1)
	{
		if (!token->next || token->next->type != 2)
			return (token->next);
		stop = token->next;
		while (stop->content[i])
		{
			if (stop->content[i] && !ft_isalnum(stop->content[i]))
			{
				str = stop->content;
				stop->content = ft_strdup(str + i);
				ft_lstadd_back(bin, ft_lstnew(stop->content));
				str[i] = '\0';
				temp = ft_lstnew_token(bin, str, 2);
				temp->next = stop;
				token->next = temp;
				return (temp);
			}
			i++;
		}
	}
	return (token->next);
}

void	ft_sepdollar(t_token *token, t_list **bin, t_token *stop)
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
				token = ft_splitdollar(token, bin, 0, stop);//PROTECT
		}
		token = ft_splitdollar(token, bin, 0, stop);//PROTECT
	}
}
