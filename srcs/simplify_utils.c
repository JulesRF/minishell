/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simplify_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jroux-fo <jroux-fo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/22 15:13:36 by jroux-fo          #+#    #+#             */
/*   Updated: 2022/06/22 15:13:36 by jroux-fo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_supempty(t_token **token)
{
	t_token	*cur;

	cur = *token;
	while (cur)
	{
		if (cur->content[0] == '\0')
			ft_delete_token(token, cur);
		cur = cur->next;
	}
}

void	ft_supspace(t_token **token)
{
	t_token	*cur;

	cur = *token;
	while (cur)
	{
		if (!ft_strcmp(cur->content, " ") && cur->type == 4)
			ft_delete_token(token, cur);
		cur = cur->next;
	}
}

void	ft_fucknorm(t_token *stop, char *str, t_list **bin, t_data *data)
{
	stop->content = ft_strdup(str);
	if (!stop->content)
	{
		clean_prog(data->env, bin, data->cmd_line);
		exit_prog("exit\nminishell: exit: bas malloc allocation", 2);
	}
	ft_lstadd_backs(bin, ft_lstnew(stop->content), data, bin);
}

void	ft_getmeout(t_data *data, t_list **bin)
{
	clean_prog(data->env, bin, data->cmd_line);
	exit_prog("exit\nminishell: exit: bas malloc allocation", 2);
}

t_token	*ft_addempty(t_token *token, t_token *stop, t_list **bin, t_data *data)
{
	token->next = ft_lstnew_token(bin, data, "", 2);
	token = token->next;
	token->next = stop;
	token = token->next;
	return (token);
}
