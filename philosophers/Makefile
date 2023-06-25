NAME	=	philo
CC		=	cc
CFLAGS	=	-Wall -Wextra -Werror
RM		=	rm -f
SRCS	=	philosophers.c eat.c init.c utils.c
OBJS	=	$(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)

re:
	make fclean
	make all

.PHONY: all clean fclean re