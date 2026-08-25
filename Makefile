# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: fkruger <marvin@42.fr>                     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/07/13 01:51:41 by fkruger           #+#    #+#              #
#    Updated: 2026/08/26 01:01:50 by fkruger          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #
NAME = philo
CC = cc
CFLAGS += -MD -Wall -Wextra -Werror -O3
CFLAGS += -g

SRCS = utils.c ph_main.c frk.c time.c run_sim.c routine.c logging.c

OBJS = $(SRCS:.c=.o)
DEPS = $(OBJS:.o=.d)

# normal targets
.PHONY: fclean clean re all
all: $(NAME)
re: clean all
fclean: clean
	$(RM) $(NAME)
clean:
	$(RM) $(OBJS) $(DEPS)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) -pthread -o $@ $+
