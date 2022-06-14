/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfiszbin <vfiszbin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/04 14:05:03 by jroux-fo          #+#    #+#             */
/*   Updated: 2022/06/14 17:13:20 by vfiszbin         ###   ########.fr       */
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
# include <sys/stat.h>
# include <unistd.h>
# include <stdlib.h>
# include <../libft/libft.h>
# include <errno.h>
# include <fcntl.h>

#include <dirent.h>

# define BUFFER_SIZE 4096

typedef struct s_token
{
	int				type; //1-operator, 2-word, 3-ponct, 4-space, 5-redir
	char			*content;
	struct s_token	*next;
}	t_token;

typedef struct s_vars
{
	t_token	**cmd;
	char	***env;
	t_list	**bin;
	pid_t	pid; 
	char	*cmd_line;
}	t_vars;

extern int g_exit_status;

//	main.c
int	main(int argc, char **argv, char **env);
int search_cmd(t_vars *vars);
void	ft_print(t_token *token);
int	ft_strcmp(char *s1, char *s2);
int echo(t_token *command);
int handle_error(char *error_msg, int ret_value, char *to_free1, char *to_free2);
int pwd();
int cd(t_token *command, char ***env);
int export(t_token *command, char ***env);
int env_builtin(char **env);
void free_strs_array(char **strs);
char *get_env_value(char *key, char **env);
int unset(t_token *command, char ***env);
void	ft_lstadd_back_token(t_token **alst, t_token *new);
int redir_and_exec(t_vars *vars);
void ft_delete_token(t_token **alst, t_token *to_del);
void free_strs(char **strs);
char **dup_env(char **envp);
void handle_sigquit(int code);
void handle_sigint(int code);
void handle_sigint_no_prompt(int code);
int	handle_errno(char *error_msg, int ret, t_token **to_free1, char **to_free2);
int exit_builtin(t_token *command, char ***env, t_list **bin, char *cmd_line);
void	ft_garbage(t_list **bin);
int add_var_to_env(char *s, char ***env);
int set_var_in_env(char *s, char *var_name, int name_len, char ***env);
int	print_sorted_env(char **env);
char	*get_var_name(char *s, int name_len);
int	var_name_is_valid(char *s, int *name_len);
char	**cmd_to_strs(t_token *command);
int	exec_cmd(t_token *command, char **env, pid_t pid);

#endif