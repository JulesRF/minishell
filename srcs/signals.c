/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfiszbin <vfiszbin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/11 11:26:25 by vfiszbin          #+#    #+#             */
/*   Updated: 2022/06/25 15:30:59 by vfiszbin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_sigquit(int code)
{
	(void)code;
	write(1, "\b \b\b \b", 6);
}

void	handle_sigquit_heredoc(int code)
{
	(void)code;
}

void	handle_sigint(int code)
{
	(void)code;
	rl_replace_line("", 0);
	write(1, "\n", 1);
	rl_on_new_line();
	rl_redisplay();
	g_exit_status = 130;
}

void	handle_sigint_no_prompt(int code)
{
	(void)code;
	g_exit_status = 130;
}

void	change_signals(int i)
{
	if (i == 1)
	{
		signal(SIGINT, handle_sigint_no_prompt);
		signal(SIGQUIT, handle_sigquit_heredoc);
	}
	else if (i == 2)
	{
		signal(SIGINT, handle_sigint);
		signal(SIGQUIT, handle_sigquit);
	}
}
