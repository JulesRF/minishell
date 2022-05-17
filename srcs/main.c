/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jroux-fo <jroux-fo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/04 14:10:11 by jroux-fo          #+#    #+#             */
/*   Updated: 2022/05/04 16:30:20 by jroux-fo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

////// utils pour bin

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
		printf("numero: %d\ntype: %d\ncontent:->%s<-\n", i, token->type, (char *)token->content);
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

int	ft_parse_word(t_token **token, t_list **bin, char *str)
{
	int	i;
	int	j;
	char *dest;

	i = 0;
	// printf ("la str :%s\n", str);
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

void	ft_parse(t_token **token, t_list **bin, char *str)
{
	int	i;

	i = 0;
	while (str[i] != '\0')
	{
		if (str[i] == '|' || str[i] == '$')
			ft_parse_operator(token, bin, str[i]);
		else if (str[i] == 39 || str[i] == 34)
			ft_parse_ponct(token, bin, str[i]);
		else if (str[i] == ' ')
			ft_lstadd_back_token(token, ft_lstnew_token(bin, " ", 4));
		else
			i = i + ft_parse_word(token, bin, str + i);
		i++;
	}
	ft_print(*token);
	printf("fin du parsing\n");
}

int	main(int argc, char **argv, char **env)
{
	char	*str;
	t_list	*bin;
	t_token	*token;

	ft_preparse(argc, argv, env);
	bin = NULL;
	token = NULL;

	str = readline("\033[95mminishell$\033[33m ");
	while (ft_strcmp(str, "exit") && str != NULL)
	{
		// printf("la string ->%s<-\n", str);
		if (str[0] != '\0')
			add_history(str);
		//parsing pur et dur (division des elements en tokens)
		ft_parse(&token, &bin, str);
		// envoie des infos a Yassine
		ft_garbage(&bin);
		ft_clean_token(&token);
		free (str);
		str = readline("\033[95mminishell$\033[33m ");
	}
	free (str);
	// ft_garbage(&bin);
	exit(0);
}