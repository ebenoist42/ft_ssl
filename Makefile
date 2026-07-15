# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ebenoist <ebenoist@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/07/12 14:59:11 by ebenoist          #+#    #+#              #
#    Updated: 2026/07/15 15:03:38 by ebenoist         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #


NAME	= ft_ssl

CC		= cc -g
CFLAGS	= -Wall -Werror -Wextra -g
SRC		= src/main.c\
		  src/libft.c\
		  src/md5.c\
		  src/parsing.c\
		  src/utils.c\
		  src/sha256.c\

OBJ		= $(SRC:.c=.o)

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) $(LIBFT) -o $(NAME) -lm

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	@rm -f $(OBJ)

fclean: clean
	@rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re