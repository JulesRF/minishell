# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: vfiszbin <vfiszbin@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/05/04 14:03:48 by jroux-fo          #+#    #+#              #
#    Updated: 2022/07/06 18:27:07 by vfiszbin         ###   ########.fr        #
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
				cd2.c\
				export.c\
				export2.c\
				export3.c\
				env.c\
				unset.c\
				redir_and_exec.c\
				redir_and_exec2.c\
				heredoc.c\
				heredoc2.c\
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
				dollar2.c\
				simplify.c\
				prompt.c\
				simplify_utils.c\
				simplify_utils2.c\
				questionmark.c\
				
FLAGS = -Wall -Werror -Wextra -g3

INCLUDES = header

INCLUDES_LIBFT = header

POTH = srcs

OBJ_DIR = obj

OBJ_FILES = $(addprefix $(OBJ_DIR)/,$(SRCS_FILES:.c=.o))

LIBFT_A = ./libft/libft.a

all: LIBFT $(NAME)
	

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

$(NAME): header/minishell.h $(OBJ_FILES)
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