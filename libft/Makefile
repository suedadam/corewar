#!/usr/bin/make -f

################################################################################
# SETTINGS                                                                     #
################################################################################

NAME = libft.a
CC = gcc
CFLAGS = -Wall -Werror -Wextra -O3 -funroll-loops -march=native -pipe
INC = -I inc
SRC_DIR = src
SRC = \
	char_print\
	char_util\
	float_print\
	float_util\
	ft_atoi\
	ft_isdigit\
	ft_islower\
	ft_isspace\
	ft_isupper\
	ft_memalloc\
	ft_memcpy\
	ft_memdel\
	ft_memmove\
	ft_memset\
	ft_printf\
	ft_printf_parser\
	ft_stpcpy\
	ft_stpncpy\
	ft_strchr\
	ft_strcmp\
	ft_strdup\
	ft_strlen\
	ft_strnlen\
	ft_strrchr\
	ft_strsub\
	ft_strupcase\
	ft_tolower\
	ft_toupper\
	get_next_line\
	int_handlers\
	int_handlers_deprecated\
	int_print\
	int_util
OBJ_DIR = $(SRC_DIR)/obj
OBJ = $(patsubst %, $(OBJ_DIR)/%.o, $(SRC))

################################################################################
# COLORS                                                                       #
################################################################################

NC = \033[0m
GREEN = \033[1;32m
RED = \033[1;31m

################################################################################
# RULES                                                                        #
################################################################################

all: $(NAME)

$(NAME): $(OBJ)
	@ar -rcs $@ $(OBJ)
	@echo "$(GREEN)DONE$(NC)"

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	@$(CC) $(CFLAGS) $(INC) -c $< -o $@

clean:
	@rm -rf $(OBJ_DIR)

fclean: clean
	@rm -f $(NAME)
	@echo "$(RED)$(NAME) removed$(NC)"

re: fclean all
