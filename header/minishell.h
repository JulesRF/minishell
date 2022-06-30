/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfiszbin <vfiszbin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/04 14:05:03 by jroux-fo          #+#    #+#             */
/*   Updated: 2022/06/26 15:49:21 by vfiszbin         ###   ########.fr       */
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
# include <dirent.h>

# define BUFFER_SIZE 4096

typedef struct s_token
{
	int				type; //1-operator, 2-word, 3-ponct, 4-space, 5-redir, 6-phantom
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

typedef struct s_redir
{
	int		tmpin;
	int		tmpout;
	int		fdin;
	int		fdout;
	int		input_redir;
	int		heredoc_redir;
	int		output_redir;
	int		fdpipe[2];
	t_token	**cmd_table;
	int		nb_cmd;
	int		ret;
	int		i;
	t_list	*heredoc_eofs;
	int		nb_heredocs;
	int		count_heredocs;
}	t_redir;

extern int g_exit_status;

//	main.c
int		main(int argc, char **argv, char **env);

//	redir_and_exec.c
int		redir_and_exec(t_vars *vars);

//	heredoc.c
int		multiple_heredoc(t_list *heredoc_eofs, int *input_redir,
			int nb_heredocs);
int		find_heredocs(t_token **commands, t_redir *redir);

//	redirections.c
int		find_in_out_files(t_token **commands, t_redir *redir);
int		add_heredoc_eof_to_list(t_token **cur, t_token **commands,
			t_redir *redir);

//	execution.c
int		exec_cmd(t_token *command, char **env, pid_t pid);
int		get_child_status(int pid, int *ret, int change_sig, int ignore_err);
int		fork_exec(t_vars *vars, t_redir *redir);

//	search_cmd.c
int		search_cmd(t_vars *vars);

//	signal.c
void	handle_sigquit(int code);
void	handle_sigint(int code);
void	handle_sigint_no_prompt(int code);
void	handle_sigquit_heredoc(int code);
void	change_signals(int i);

//	utils
char	*get_env_value(char *key, char **env);
int		handle_error(char *error_msg, int ret_value, char *to_free1,
			char *to_free2);
int		handle_errno(char *error_msg, int ret, t_token **to_free1,
			t_list **to_free2);
char	*get_env_var(char *key, char **env);

//	utils2.c
char	**cmd_to_strs(t_token *command);
void	free_strs(char **strs);
int		get_nb_cmd_and_heredocs(t_token *commands, int *nb_heredocs);
t_token	**split_commands(t_token *commands, int nb_cmd, int i);

//	garbage.c
void	ft_garbage(t_list **bin);
t_list	*ft_lstnew(void *content);
t_list	*ft_lstlast(t_list *lst);
void	ft_lstadd_back(t_list **alst, t_list *new);

//	token_utils.c
t_token	*ft_lstnew_token(t_list **bin, char *content, int type);
t_token	*ft_lstlast_token(t_token *lst);
void	ft_lstadd_back_token(t_token **alst, t_token *new);
void	ft_delete_token(t_token **alst, t_token *to_del);
void	ft_clean_token(t_token **token);

//	parse_utils.c
char	*ft_strdup(const char *s1);
int		ft_strcmp(char *s1, char *s2);
void	ft_preparse(int argc, char **argv, char **env);
void	ft_print(t_token *token);

//	token.c
void	ft_parse_operator(t_token **token, t_list **bin, char c);
void	ft_parse_ponct(t_token **token, t_list **bin, char c);
int		ft_parse_redir(t_token **token, t_list **bin, char c, char *str);
int		ft_parse_word(t_token **token, t_list **bin, char *str);
void	ft_token(t_token **token, t_list **bin, char *str);

//	quotes.c
t_token	*ft_joincontent(t_token *temp, t_token *token, t_list **bin);
t_token	*ft_find_dquote(t_token *token);
void	ft_doublequotes(t_token *token, t_list **bin, t_token *temp,
			t_token *stop);
t_token	*ft_find_squote(t_token *token);
void	ft_simplequotes(t_token *token, t_list **bin, t_token *temp,
			t_token *stop);

//	dollar.c
void	ft_dollarfind(t_token *token, char *to_find, char **env, t_list **bin);
t_token	*ft_isdollar(t_token *token, t_list **bin, char **env);
void	ft_dollar(t_token *token, t_list **bin, char **env);
t_token	*ft_splitdollar(t_token *token, t_list **bin, int i, t_token *stop);
void	ft_sepdollar(t_token *token, t_list **bin, t_token *stop);

//	dollar2.c
int		ft_dollarcheck(t_token *token, char *to_find, char **env, t_list **bin);

//	simplify.c
int		ft_piperedir(t_token *token, t_list **bin);
void	ft_rmvquotes(t_token **token, t_list **bin);
void	ft_joinwords(t_token **token, t_list **bin);
int		ft_first_quote(t_token *token);
int		ft_simplify(t_token **token, t_list **bin, char **env);

//	prompt.c
int		ft_closed_quotes(char *str, int i);
int		ft_syntax(char *str, t_list **bin);
void	ft_prompt(t_token **token, t_list **bin, char ***env, char *tester_cmd);
char	**dup_env(char **envp);
void	free_strs_array(char **strs);

//	questionmark.c
void	ft_supempty(t_token **token);
void	ft_questionmark(t_token *token, t_list **bin);

//	simplify_utils.c
void	ft_supspace(t_token **token);
t_token	*ft_addempty(t_token *token, t_token *stop, t_list **bin);

////	builtins	////
//	echo.c
int		echo(t_token *command);

//	cd.c
int		cd(t_token *command, char ***env);

//	cd2.c
int		unset_oldpwd(char ***env);
int		get_current_pwd(char **new_str);
int		path_exists(char *path);
int		set_new_dir(char **path, char **old_pwd, char **env);

//	pwd.c
int		pwd(void);

//	export.c
int		export(t_token *command, char ***env);
int		add_var_to_env(char *s, char ***env);
int		set_var_in_env(char *s, char *var_name, int name_len, char ***env);

//	export2.c
int		print_sorted_env(char **env);
char	*get_var_name(char *s, int name_len);
int		var_name_is_valid(char *s, int *name_len);

//export3.c
int		concat_to_env(t_token *command, char ***env, int name_len);

//	env.c
int		env_builtin(char **env);

//	exit.c
int		exit_builtin(t_token *command, char ***env, t_list **bin,
			char *cmd_line);

//	unset.c
int		unset(t_token *command, char ***env);
int		rm_var_from_env(char *s, char ***env);
////	////	////

#endif