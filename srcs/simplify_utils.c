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
	t_token *cur;

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
	t_token *cur;

	cur = *token;
	while (cur)
	{
		if (!ft_strcmp(cur->content, " ") && cur->type == 4)
			ft_delete_token(token, cur);
		cur = cur->next;
	}
}

t_token	*ft_addempty(t_token *token, t_token *stop, t_list **bin)
{
	token->next = ft_lstnew_token(bin, "", 2);
	token = token->next;
	token->next = stop;
	token = token->next;
	return (token);
}
