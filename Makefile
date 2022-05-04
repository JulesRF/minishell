# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jroux-fo <jroux-fo@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/05/04 14:03:48 by jroux-fo          #+#    #+#              #
#    Updated: 2022/05/04 15:35:06 by jroux-fo         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell

SRCS_FILES	=	main.c	\

FLAGS = -Wall -Werror -Wextra -g3

INCLUDES = header

PATH = srcs

OBJ_DIR = obj

OBJ_FILES = $(addprefix $(OBJ_DIR)/,$(SRCS_FILES:.c=.o))

all: $(NAME)

$(OBJ_DIR)/%.o: $(PATH)/%.c
	@$(shell mkdir -p $(OBJ_DIR))
	@printf "Compiling $< ...\n"
	@$(shell gcc $(FLAGS) -I $(INCLUDES) -c $< -o $@)

$(OBJ_DIR)/%.o: $(PATH)/%.c
	@$(shell mkdir -p $(OBJ_DIR))
	@printf "Compiling $< ...\n"
	@$(shell gcc $(FLAGS) -I $(INCLUDES) -c $< -o $@)

$(NAME): $(OBJ_FILES)
	@$(shell gcc $(FLAGS) -I $(INCLUDES) $(OBJ_FILES) -lreadline -o $(NAME))
	@printf "Executable $@ created !\n"

clean:
	@printf "Deleting objects\n"
	@$(shell rm -rf $(OBJ_DIR))

fclean: clean
	@printf "Deleting executable\n"
	@$(shell rm -rf $(NAME))
	
re: fclean all

.PHONY:		all clean fclean re