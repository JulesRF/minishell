/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfiszbin <vfiszbin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/04 14:10:11 by jroux-fo          #+#    #+#             */
/*   Updated: 2022/06/07 15:28:17 by vfiszbin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_strlen2(char *str)
{
	int i;

	i = 0;
	if (!str)
		return (0);
	while (str[i])
		i++;
	return (i);
}

void	ft_garbage(t_list **bin)
{
	t_list	*to_suppr;

	while (*bin)
	{
		to_suppr = *bin;
		*bin = to_suppr->next;
		free(to_suppr->content);
		free(to_suppr);
	}
	*bin = NULL;
}

////// utils pour bin (garbage collector)

t_list	*ft_lstnew(void *content)
{
	t_list	*newcell;

	newcell = malloc(sizeof(t_list));
	if (!newcell)
		return (NULL);
	newcell->content = content;
	newcell->next = NULL;
	return (newcell);
}

t_list	*ft_lstlast(t_list *lst)
{
	if (!lst)
		return (NULL);
	while (lst->next)
		lst = lst->next;
	return (lst);
}

void	ft_lstadd_back(t_list **alst, t_list *new)
{
	t_list	*temp;

	if (!*alst)
	{
		*alst = new;
		return ;
	}
	temp = ft_lstlast(*alst);
	temp->next = new;
}

////// utils pour bin

////// utils pour token

t_token	*ft_lstnew_token(t_list **bin, char *content, int type)
{
	t_token	*newcell;

	newcell = malloc(sizeof(t_token));
	ft_lstadd_back(bin, ft_lstnew(newcell));
	if (!newcell)
		return (NULL);
	newcell->content = content;
	newcell->type = type;
	newcell->next = NULL;
	return (newcell);
}

t_token	*ft_lstlast_token(t_token *lst)
{
	if (!lst)
		return (NULL);
	while (lst->next)
		lst = lst->next;
	return (lst);
}

void	ft_lstadd_back_token(t_token **alst, t_token *new)
{
	t_token	*temp;

	if (!*alst)
	{
		*alst = new;
		return ;
	}
	temp = ft_lstlast_token(*alst);
	temp->next = new;
}

////// utils pour token

char	*ft_strdup(const char *s1)
{
	char	*dest;
	int		i;

	i = 0;
	while (s1[i])
		i++;
	dest = malloc(sizeof(char) * i + 1);
	if (!dest)
		return (NULL);
	i = 0;
	while (s1[i])
	{
		dest[i] = s1[i];
		i++;
	}
	dest[i] = '\0';
	return (dest);
}

int	ft_strcmp(char *s1, char *s2)
{
	int	i;

	i = 0;
	while (s1[i] || s2[i])
	{
		if (s1[i] - s2[i] != 0)
			return (s1[i] - s2[i]);
		i++;
	}
	return (0);
}

int	ft_strncmp2(const char *s1, const char *s2, int n)
{
	int	i;

	i = 0;
	while (((unsigned char)s1[i] || (unsigned char)s2[i]) && i < n)
	{
		if ((unsigned char)s1[i] != (unsigned char)s2[i])
			return ((unsigned char)s1[i] - (unsigned char)s2[i]);
		i++;
	}
	return (0);
}

void	ft_preparse(int argc, char **argv, char **env)
{
	(void)argv;
	(void)env;
	if (argc != 1)
	{
		printf("no need to put arguments, simply type: ./minishell\n");
		exit(1);
	}
	return ;
}

void	ft_print(t_token *token)
{
	int	i;

	printf("printing token list...\n");
	i = 1;
	while (token)
	{
		printf("---------------\n");
		printf("numero: %d\ntype: %d\ncontent:\e[38;5;196m->\033[0m%s\e[38;5;196m<-\033[0m\n", i, token->type, (char *)token->content);
		printf("---------------\n");
		i++;
		token = token->next;
	}
}

void	ft_clean_token(t_token **token)
{
	// t_token *to_suppr;

	// while (*token)
	// {
	// 	to_suppr = *token;
	// 	*token = to_suppr->next;
	// 	//free si pas | ou si pas $
	// 	free(to_suppr);
	// }
	*token = NULL;
}

int	ft_closed_quotes(char *str, t_list **bin)
{
	int	i;

	(void)bin;
	i = 0;
	while (str[i])
	{
		if (str[i] == '\'')
		{
			i++;
			while (str[i] && str[i] != '\'')
				i++;
			if (str[i] != '\'')
				return (printf("SYNTAX ERROR\n"), 1);
		}
		if (str[i] == '\"')
		{
			i++;
			while (str[i] && str[i] != '\"')
				i++;
			if (str[i] != '\"')
				return (printf("SYNTAX ERROR\n"), 1);
		}
		i++;
	}
	return (0);
}

int	ft_syntax(char *str, t_list **bin)
{
	if (ft_closed_quotes(str, bin))
		return (1);
	return (0);
}

void	ft_parse_operator(t_token **token, t_list **bin, char c)
{
	if (c == '|')
		ft_lstadd_back_token(token, ft_lstnew_token(bin, "|", 1));
	if (c == '$')
		ft_lstadd_back_token(token, ft_lstnew_token(bin, "$", 1));
}

void	ft_parse_ponct(t_token **token, t_list **bin, char c)
{
	if (c == 39)
		ft_lstadd_back_token(token, ft_lstnew_token(bin, "\'", 3));
	if (c == 34)
		ft_lstadd_back_token(token, ft_lstnew_token(bin, "\"", 3));
}

int	ft_parse_redir(t_token **token, t_list **bin, char c, char *str)
{
	if (c == 60)
	{
		if (str[1] == 60)
		{
			ft_lstadd_back_token(token, ft_lstnew_token(bin, "<<", 5));
			return (1);
		}
		else
			ft_lstadd_back_token(token, ft_lstnew_token(bin, "<", 5));
	}
	if (c == 62)
	{
		if (str[1] == 62)
		{
			ft_lstadd_back_token(token, ft_lstnew_token(bin, ">>", 5));
			return (1);
		}
		else
			ft_lstadd_back_token(token, ft_lstnew_token(bin, ">", 5));
	}
	return (0);
}

int	ft_parse_word(t_token **token, t_list **bin, char *str)
{
	int	i;
	int	j;
	char *dest;

	i = 0;
	j = 0;
	while (str[i] != '|' && str[i] != '$' && str[i] != 39 && str[i] != 34
		&& str[i] != ' ' && str[i])
		i++;
	dest = malloc(sizeof(char) * i + 1);
	ft_lstadd_back(bin, ft_lstnew(dest));
	while (j < i)
	{
		dest[j] = str[j];
		j++;
	}
	dest[j] = '\0';
	ft_lstadd_back_token(token, ft_lstnew_token(bin, dest, 2));
	return (i - 1);
}

void	ft_token(t_token **token, t_list **bin, char *str)
{
	int	i;

	i = 0;
	while (str[i] != '\0')
	{
		if (str[i] == '|' || str[i] == '$')
			ft_parse_operator(token, bin, str[i]);
		else if (str[i] == 39 || str[i] == 34)
			ft_parse_ponct(token, bin, str[i]);
		else if (str[i] == 60 || str[i] == 62)
			i = i + ft_parse_redir(token, bin, str[i], str + i);
		else if (str[i] == ' ')
			ft_lstadd_back_token(token, ft_lstnew_token(bin, " ", 4));
		else
			i = i + ft_parse_word(token, bin, str + i);
		i++;
	}
	// ft_print(*token);
	// printf("fin du parsing\n");
}

void	ft_supspace(t_token *token)
{
	while (token)
	{
		if (token->next != NULL && !ft_strcmp(token->content, " ") && !ft_strcmp(token->next->content, " "))
			token->next = token->next->next;
		else 
			token = token->next;
	}
}

t_token	*ft_joincontent(t_token *temp, t_token *token, t_list **bin)
{
	char	*str;
	int		i;
	int		j;

	if (temp == NULL)
		return (ft_lstnew_token(bin, token->content, 5));
	str = malloc(sizeof(char) * (ft_strlen2(temp->content) + ft_strlen2(token->content)) + 1);
	ft_lstadd_back(bin, ft_lstnew(str));
	i = 0;
	j = 0;
	while (temp->content[i])
	{
		str[i] = temp->content[i];
		i++;
	}
	while (token->content[j])
	{
		str[i] = token->content[j];
		i++;
		j++;
	}
	str[i] = '\0';
	temp->content = str;
	return (temp);
}

t_token	*ft_find_dquote(t_token *token)
{
	while (token)
	{
		if (!ft_strcmp(token->content, "\""))
			return(token);
		token = token->next;
	}
	return (token);
}

void	ft_doublequotes(t_token *token, t_list **bin, t_token *temp,
t_token *stop)
{
	temp = NULL;
	while (token)
	{
		if (!ft_strcmp(token->content, "\"") && token->type == 3)
		{
			stop = token->next;
			if (!ft_strcmp(stop->content, "\"") && stop->type == 3)
				token = token->next;
			else
			{
				while (stop && ft_strcmp(stop->content, "\""))
				{
					temp = ft_joincontent(temp, stop, bin);
					stop = stop->next;
				}
				temp->next = stop;
				token->next = temp;
				token = ft_find_dquote(temp);
				ft_doublequotes(token->next, bin, temp, stop);
				return ;
			}
		}
		token = token->next;
	}
}

t_token	*ft_find_squote(t_token *token)
{
	while (token)
	{
		if (!ft_strcmp(token->content, "\'"))
			return(token);
		token = token->next;
	}
	return (token);
}

void	ft_simplequotes(t_token *token, t_list **bin, t_token *temp,
t_token *stop)
{
	temp = NULL;
	while (token)
	{
		if (!ft_strcmp(token->content, "\'") && token->type == 3)
		{
			stop = token->next;
			if (!ft_strcmp(stop->content, "\'") && stop->type == 3)
				token = token->next;
			else
			{
				while (stop && ft_strcmp(stop->content, "\'"))
				{
					temp = ft_joincontent(temp, stop, bin);
					stop = stop->next;
				}
				temp->next = stop;
				token->next = temp;
				token = ft_find_squote(temp);
				ft_simplequotes(token->next, bin, temp, stop);
				return ;
			}
		}
		token = token->next;
	}
}

char	*ft_dollarfind(char *to_find, char **env)
{
	int	i;

	i = 0;
	if (!ft_strcmp(to_find, " "))
		return ("$ ");
	while (env[i])
	{
		if (!ft_strncmp2(env[i], to_find, ft_strlen2(to_find)))
			return (env[i] + (ft_strlen2(to_find) + 1));
		i++;
	}
	return ("\n");
}

void	ft_dollar(t_token *token, t_list **bin, char **env)
{
	(void)bin;
	while (token)
	{
		if (!ft_strcmp(token->content, "\'") && token->type == 3)
		{
			token = token->next;
			while (token && ft_strcmp(token->content, "\'"))
				token = token->next;
		}
		if (!ft_strcmp(token->content, "$") && token->type == 1)
		{
			token->content = ft_dollarfind(token->next->content, env);
			token->type = 2;
			token->next = token->next->next;
		}
		token = token->next;
	}
}

void	ft_simplify(t_token **token, t_list **bin, char **env)
{
	t_token	*temp;
	t_token	*stop;

	temp = NULL;
	stop = NULL;
	ft_dollar(*token, bin, env);             // export : remplacer $USER par -> jroux-fo (avec env)
	ft_doublequotes(*token, bin, temp, stop);// simplifier tout les tokens entre doubles quotes par un seul token mot
	ft_simplequotes(*token, bin, temp, stop);// simplifier tout les tokens entre simple quotes par un seul token mot
	ft_supspace(*token);                     // supprimer les tokens espace en trop : "salut     ca va" -> "salut ca va"
}

int	ft_prompt(t_token **token, t_list **bin, char ***env, char *tester_cmd)
{
	char *str;
	
	str = readline("\033[95mminishell$\033[0m ");
	int ret = 0;
	(void)tester_cmd;
	// str = tester_cmd;
	while (ft_strcmp(str, "exit") && str != NULL)
	{
		if (str[0] != '\0')
			add_history(str);   // gere l'historique des commandes, sauf si la commande est un \n
		if (!ft_syntax(str, bin))
		{
			ft_token(token, bin, str); //parsing pur et dur (division des elements en tokens)
			ft_simplify(token, bin, *env); //simplification des tokens
			// ft_print(*token);			// print simplement la liste de token pour voir le resultat du parsing
			// envoie des infos a mon mate

			ret = search_cmd(*token, env);
			// printf("ret search_cmd=%d\n", ret);
			// return ret;
		}
		ft_garbage(bin);
		ft_clean_token(token);
		// free (str);
		str = readline("\033[95mminishell$\033[0m ");
	}
	// free (str);
	return ret;
}


/**
 * @brief Duplicate the environment variables into an allocated array of strings
 * 
 * @param envp Environment variables
 * @return char** NULL if malloc fails, the duplicated array of strings otherwise
 */
char **dup_env(char **envp)
{
	int		i;
	int		j;
	char	**env;

	i = 0;
	while (envp[i])
		i++;
	env = malloc(sizeof(char *) * (i + 1));
	if (!env)
		return (NULL);
	i = 0;
	while (envp[i])
	{
		env[i] = ft_strdup(envp[i]);
		if (!env[i])
		{
			j = 0;
			while (j < i)
			{
				free(env[j]);
				j++;
			}
			free(env);
			return (NULL);
		}
		i++;
	}
	env[i] = NULL;
	return (env);
}

/**
 * @brief Free an array of strings
 * 
 * @param strs Allocated array of strings
 */
void free_strs_array(char **strs)
{
	int i;

	i = 0;
	while (strs[i])
	{
		free(strs[i]);
		i++;
	}
	free(strs); //est ce qu'on ne devrait pas avoir char ***strs pour ici ?
}

int	main(int argc, char **argv, char **envp)
{
	t_list	*bin;      // garbage collector, tout ce que je malloc, je le fous dedans
	t_token	*token;
	char **env;

	ft_preparse(argc, argv, envp);
	bin = NULL;
	token = NULL;

	env = dup_env(envp);
	if (!env)
		exit(1); // ?

	// if (argc >= 3 && !ft_strncmp(argv[1], "-c", 3))
	// {
	// 	int exit_status = ft_prompt(&token, &bin, env, argv[2]);
	// 	exit(exit_status);
	// }
	
	ft_prompt(&token, &bin, &env, argv[2]);
	
	free_strs_array(env);
	ft_garbage(&bin);
	exit(0);
}