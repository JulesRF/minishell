# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: vfiszbin <vfiszbin@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/05/04 14:03:48 by jroux-fo          #+#    #+#              #
#    Updated: 2022/06/04 12:33:15 by vfiszbin         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell

SRCS_FILES	=	main.c	\
				execution.c\

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

$(NAME): $(OBJ_FILES)
	${MAKE} -C ./libft
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