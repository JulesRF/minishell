# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jroux-fo <jroux-fo@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/05/04 14:03:48 by jroux-fo          #+#    #+#              #
#    Updated: 2022/06/22 15:53:39 by jroux-fo         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell

SRCS_FILES	=	main.c	\
				execution.c\
				search_cmd.c\
				exit.c\
				echo.c\
				pwd.c\
				cd.c\
				export.c\
				export2.c\
				export3.c\
				env.c\
				unset.c\
				redir_and_exec.c\
				heredoc.c\
				redirections.c\
				signals.c\
				utils.c\
				utils2.c\
				garbage.c\
				token_utils.c\
				parse_utils.c\
				token.c\
				quotes.c\
				dollar.c\
				simplify.c\
				prompt.c\
				simplify_utils.c\
				
FLAGS = -Wall -Werror -Wextra -g3

INCLUDES = header

INCLUDES_LIBFT = header

POTH = srcs

OBJ_DIR = obj

OBJ_FILES = $(addprefix $(OBJ_DIR)/,$(SRCS_FILES:.c=.o))

LIBFT_A = ./libft/libft.a

all: $(NAME)
	

$(OBJ_DIR)/%.o: $(POTH)/%.c
	@$(shell mkdir -p $(OBJ_DIR))
	@printf "Compiling $< ...\n"
	@$(shell gcc $(FLAGS) -I $(INCLUDES) -c $< -o $@)

$(OBJ_DIR)/%.o: $(POTH)/%.c
	@$(shell mkdir -p $(OBJ_DIR))
	@printf "Compiling $< ...\n"
	@$(shell gcc $(FLAGS) -I $(INCLUDES) -c $< -o $@)

LIBFT:
	${MAKE} -C ./libft

$(NAME): LIBFT $(OBJ_FILES)
	@$(shell gcc $(FLAGS) -I $(INCLUDES) -I $(INCLUDES_LIBFT) $(OBJ_FILES) $(LIBFT_A) -lreadline -o $(NAME))
	@printf "Executable $@ created !\n"

clean:
	${MAKE} clean -C ./libft
	@printf "Deleting objects\n"
	@$(shell rm -rf $(OBJ_DIR))


fclean: clean
	${MAKE} fclean -C ./libft
	@printf "Deleting executable\n"
	@$(shell rm -rf $(NAME))

	
re: fclean all

.PHONY:		all clean fclean re