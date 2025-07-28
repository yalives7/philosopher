# Makefile
NAME = philo

CC = cc
CFLAGS = -Wall -Wextra -Werror -Iinclude -pthread
RM = rm -f

SRCS = \
	src/main.c \
	src/init.c \
	src/start.c \
	src/philo.c \
	src/routine.c \
	src/routine2.c \
	src/validation.c \
	src/utils.c \
	src/cleanup.c\
	src/monitor.c

OBJS = $(SRCS:.c=.o)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS)

all: $(NAME)

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re
