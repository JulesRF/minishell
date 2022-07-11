/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dollar2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jroux-fo <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/30 15:58:49 by jroux-fo          #+#    #+#             */
/*   Updated: 2022/06/30 15:58:50 by jroux-fo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_dollarcheck(t_token *token, char *to_find, char **env, t_list **bin)
{
	(void)env;
	(void)bin;
	if (!ft_strcmp(to_find, " "))
	{
		token->content = "$ ";
		return (1);
	}
	if (to_find && (to_find[0] >= '0' && to_find[0] <= '9'))
	{
		token->type = 2;
		token->content = to_find + 1;
		return (1);
	}
	return (0);
}

void	ft_cleantype(t_token *token)
{
	while (token)
	{
		if (token->type == 3)
			token->type = 2;
		token = token->next;
	}
}

void	ft_dollardoc(t_token *token, t_list **bin, t_data *data)
{
	ft_cleantype(token);
	while (token)
	{
		if (token && (!ft_strcmp(token->content, "$") && token->type == 1))
		{
			if (!token->next)
				return ;
			if (!ft_strcmp(token->next->content, "?"))
			{
				token->content = ft_itoa(g_exit_status);
				if (!token->content)
					return ;
				ft_lstadd_backs(bin, ft_lstnew(token->content), data);
				token->type = 2;
			}
			else
				ft_dollarfind(token, token->next->content, data, bin);
			token->next = token->next->next;
		}
		token = token->next;
	}
}

// int	ft_isspace(char *str)
// {
// 	int i;

// 	if (!str)
// 		return (-5);
// 	while (str[i])
// 	{
// 		if (str[i] == ' ')
// 			return
// 	}
// }

// void	ft_splitres(t_token *token, t_list **bin, t_data *data)
// {
// 	int	i;
// 	while (token)
// 	{
// 		i = ft_isspace(token->content);
// 	}
// }
