# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: xxxxxxxx <xxxxxxxx@student.42berlin.de>    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/11/10 15:34:24 by xxxxxxxx          #+#    #+#              #
#    Updated: 2024/12/03 16:37:57 by xxxxxxxx         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = pipex
SRCS = pipex.c \
	pipex_children.c \
	pipex_utils.c
OBJS = $(SRCS:.c=.o)
BONUS_SRCS = pipex_bonus.c \
	pipex_bonus_children.c \
	pipex_bonus_utils.c 
BONUS_OBJS = $(BONUS_SRCS:.c=.o)
CC = cc
CFLAGS = -Wall -Wextra -Werror
INCLUDES = -Iinclude -Ilibft/include
LIBS = -Llibft -lft
LIBFT = libft
LIBFT_MAKE = make bonus
VPATH = src bonus

all: $(NAME)

$(NAME): $(OBJS)
	$(LIBFT_MAKE) -C $(LIBFT)
	$(CC) $(CFLAGS) $(OBJS) $(LIBS) -o $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

bonus: $(BONUS_OBJS)
	$(LIBFT_MAKE) -C $(LIBFT)
	$(CC) $(CFLAGS) $(BONUS_OBJS) $(LIBS) -o $(NAME)

clean:
	make clean -C $(LIBFT) 
	rm -f $(OBJS) $(BONUS_OBJS)

fclean: clean
	make fclean -C $(LIBFT)
	rm -f $(NAME)

re: fclean all

debug: CFLAGS += -g
debug: LIBFT_MAKE = make debug
debug: fclean bonus

.PHONY: all clean fclean re bonus debug
