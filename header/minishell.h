/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jroux-fo <jroux-fo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/04 14:05:03 by jroux-fo          #+#    #+#             */
/*   Updated: 2022/05/19 16:11:29 by jroux-fo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H
# include <stdio.h>
# include <signal.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <unistd.h>
# include <stdlib.h>

typedef struct s_list
{
	void    		*content;
	struct s_list	*next;
}					t_list;

typedef struct s_token
{
	int				type; //1-operator, 2-word, 3-ponct, 4-space, 5-redir
	char			*content;
	struct s_token	*next;
}					t_token ;
//	main.c
int	main(int argc, char **argv, char **env);

#endif