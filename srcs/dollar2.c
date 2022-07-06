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
