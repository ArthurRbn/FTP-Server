##
## EPITECH PROJECT, 2019
## Makefile
## File description:
## Makefile
##

SRC		=		src/main.c						\
				src/server.c					\
				src/client.c					\
				src/utils.c						\
				src/filesystem_utils.c			\
				src/commands.c					\
				src/directory_commands.c		\
				src/passive_mode.c				\
				src/active_mode.c				\
				src/list_retr_cmd.c				\
				src/data_commands.c				\

RM		=		rm -rf

NAME	=		myftp

OBJ		=		$(SRC:.c=.o)

CFLAGS	=		-Wall -Werror -Wextra

$(NAME):	$(OBJ)
			gcc -g $(CFLAGS) -o $(NAME) $(SRC)

all:		$(NAME)

clean:
			@$(RM) $(OBJ)

fclean:		clean
			@$(RM) $(NAME)

re:			fclean all