/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfiszbin <vfiszbin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/11 11:26:25 by vfiszbin          #+#    #+#             */
/*   Updated: 2022/06/15 18:11:15 by vfiszbin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void handle_sigquit(int code)
{
	(void)code;
	g_exit_status = 131;
	//réécrire le buffer d'avant signal pour effacer ^\ ?
}

void handle_sigint(int code)
{
	(void)code;
	rl_replace_line("", 0);
	write(1, "\n", 1);
	rl_on_new_line();
	rl_redisplay();
	g_exit_status = 130;

}

void handle_sigint_no_prompt(int code)
{
	(void)code;
	write(1, "\n", 1);
	g_exit_status = 130;
}