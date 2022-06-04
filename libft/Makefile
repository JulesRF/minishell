# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: vfiszbin <vfiszbin@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/11/22 19:49:42 by vfiszbin          #+#    #+#              #
#    Updated: 2022/05/30 19:42:18 by vfiszbin         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SRCS = 	ft_isdigit.c\
		ft_isalpha.c\
		ft_isalnum.c\
		ft_isascii.c\
		ft_isprint.c\
		ft_strlen.c\
		ft_memset.c\
		ft_bzero.c\
		ft_memcpy.c\
		ft_memmove.c\
		ft_substr.c\
		ft_strlcpy.c\
		ft_strlcat.c\
		ft_toupper.c\
		ft_tolower.c\
		ft_strchr.c\
		ft_strrchr.c\
		ft_strncmp.c\
		ft_memchr.c\
		ft_memcmp.c\
		ft_strnstr.c\
		ft_atoi.c\
		ft_calloc.c\
		ft_strdup.c\
		ft_strjoin.c\
		ft_strtrim.c\
		ft_split.c\
		ft_itoa.c\
		ft_strmapi.c\
		ft_striteri.c\
		ft_putchar_fd.c\
		ft_putstr_fd.c\
		ft_putendl_fd.c\
		ft_putnbr_fd.c\
		ft_lstnew.c\
		ft_lstadd_front.c\
		ft_lstsize.c\
		ft_lstlast.c\
		ft_lstadd_back.c\
		ft_lstdelone.c\
		ft_lstclear.c\
		ft_lstiter.c\
		ft_lstmap.c\
		get_next_line.c\
		get_next_line_utils.c\
		ft_print_uint_fd.c\
		ft_printchar_fd.c\
		ft_printnbr_base.c\
		ft_printnbr_fd.c\
		ft_printptr_base.c\
		ft_printstr_fd.c\
		ft_printf.c\

OBJS = ${SRCS:.c=.o}

NAME = libft.a

CC = gcc
RM = rm -f

BUFFER_SIZE = 42

CFLAGS = -Wall -Wextra -Werror

.c.o:
	${CC} ${CFLAGS} -D BUFFER_SIZE=${BUFFER_SIZE} -c $ $< -o ${<:.c=.o}


${NAME}: ${OBJS}
	ar rc ${NAME} ${OBJS}

all: ${NAME}

clean:
	${RM} ${OBJS}

fclean: clean
	${RM} ${NAME}

re: fclean all

.PHONY: all clean fclean re