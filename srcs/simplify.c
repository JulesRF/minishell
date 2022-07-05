/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simplify.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jroux-fo <jroux-fo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/22 15:04:22 by jroux-fo          #+#    #+#             */
/*   Updated: 2022/06/22 15:04:22 by jroux-fo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_piperedir(t_token *token, t_list **bin)
{
	(void)bin;
	if (!ft_strcmp(token->content, "|") && token->type == 1)
		return (printf("minishell: syntax error\n"), 1);
	while (token)
	{
		if ((!ft_strcmp(token->content, "|") && token->type == 1)
			|| (token->type == 5))
		{
			if (!token->next)
				return (printf("minishell: syntax error\n"), 1);
			if (token->next->type == 4)
				token = token->next;
			if (!token->next)
				return (printf("minishell: syntax error\n"), 1);
			if (token->next->type != 2 && token->next->type != 3
				&& token->next->type != 5 && token->next->type != 6)
				return (printf("minishell: syntax error\n"), 1);
		}
		token = token->next;
	}
	return (0);
}

void	ft_rmvquotes(t_token **token, t_list **bin)
{
	t_token	*tmp;

	(void)bin;
	tmp = *token;
	while (tmp)
	{
		if ((!ft_strcmp(tmp->content, "\"") && tmp->type == 3)
			|| (!ft_strcmp(tmp->content, "\'") && tmp->type == 3))
			ft_delete_token(token, tmp);
		tmp = tmp->next;
	}
}

void	ft_joinwords(t_token **token, t_list **bin, t_data *data)
{
	t_token	*tmp;

	tmp = *token;
	while (tmp)
	{
		if (tmp->type == 2 && tmp->next && tmp->next->type == 2)
		{
			tmp = ft_joincontent(tmp, tmp->next, bin, data);
			tmp->next = tmp->next->next;
		}
		else
			tmp = tmp->next;
	}
}

int	ft_first_quote(t_token *token)
{
	while (token)
	{
		if (!ft_strcmp(token->content, "\"") && token->type == 3)
			return (1);
		if (!ft_strcmp(token->content, "\'") && token->type == 3)
			return (0);
		token = token->next;
	}
	return (0);
}

int	ft_simplify(t_token **token, t_list **bin, t_data *data)
{
	data->temp = NULL;
	data->stop = NULL;
	ft_questionmark(*token, bin, data);
	ft_sepdollar(*token, bin, data);
	ft_supempty(token);
	ft_dollar(*token, bin, data);             // export : remplacer $USER par -> jroux-fo (avec env)
	// ft_print(*token);
	if (ft_first_quote(*token))
	{
		ft_doublequotes(*token, bin, data, NULL);//temp, stop);
		ft_simplequotes(*token, bin, data, NULL);//temp, stop);
	}
	else
	{
		ft_simplequotes(*token, bin, data, NULL);//temp, stop);
		ft_doublequotes(*token, bin, data, NULL);//temp, stop);
	}
	// ft_doublequotes(*token, bin, temp, stop);// simplifier tout les tokens entre doubles quotes par un seul token mot
	// ft_simplequotes(*token, bin, temp, stop);// simplifier tout les tokens entre simple quotes par un seul token mot
	// ft_print(*token);
	ft_rmvquotes(token, bin);
	ft_joinwords(token, bin, data);
	ft_supspace(token);                     // supprimer les tokens espace en trop : "salut     ca va" -> "salut ca va"
	return (0);
}
