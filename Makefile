# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ebenoist <ebenoist@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/06/30 11:11:41 by ebenoist          #+#    #+#              #
#    Updated: 2026/07/13 11:03:05 by ebenoist         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #



NAME	= ft_ssl

CC		= cc -g
CFLAGS	= -Wall -Werror -Wextra -g
SRC		= src/main.c\
		  src/libft.c\
		  src/md5.c\


			 
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