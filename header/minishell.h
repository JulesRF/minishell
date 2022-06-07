/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfiszbin <vfiszbin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/04 14:05:03 by jroux-fo          #+#    #+#             */
/*   Updated: 2022/06/07 15:28:58 by vfiszbin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H
# include <stdio.h>
# include <signal.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <sys/types.h>
# include <sys/wait.h>
 #include <sys/stat.h>
# include <unistd.h>
# include <stdlib.h>
# include <../libft/libft.h>
# include <errno.h>

# define BUFFER_SIZE 4096

typedef struct s_token
{
	int				type; //1-operator, 2-word, 3-ponct, 4-space, 5-redir
	char			*content;
	struct s_token	*next;
}					t_token ;
//	main.c
int	main(int argc, char **argv, char **env);
int search_cmd(t_token *command, char ***env);
void	ft_print(t_token *token);
int	ft_strcmp(char *s1, char *s2);
int echo(t_token *command);
int handle_error(char *error_msg, int ret_value);
int pwd();
int cd(t_token *command);
int export(t_token *command, char ***env);
int env_builtin(char **env);
void free_strs_array(char **strs);
char *get_env_value(char *key, char **env);
int unset(t_token *command, char ***env);

#endif