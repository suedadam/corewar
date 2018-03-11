#!/usr/bin/make -f

################################################################################
# SETTINGS                                                                     #
################################################################################

ASM = asm
COREWAR = corewar
CC = gcc
CFLAGS += -Wall -Werror -Wextra
INC = -I inc -I libft/inc
LIBFT = libft/libft.a
SRC_DIR = src
ASM_SRC = \
	asm\
	asm_fsm\
	asm_header\
	asm_label\
	asm_load\
	asm_op\
	asm_token\
	asm_util\
	asm_validate_arg\
	asm_write
CORE_SRC = 
OBJ_DIR = $(SRC_DIR)/obj
CORE_OBJ = $(patsubst %, $(OBJ_DIR)/%.o, $(CORE_SRC))
ASM_OBJ = $(patsubst %, $(OBJ_DIR)/%.o, $(ASM_SRC))

################################################################################
# COLORS                                                                       #
################################################################################

NC = \033[0m
GREEN = \033[1;32m
RED = \033[1;31m
YELLOW = \033[1;33m

################################################################################
# RULES                                                                        #
################################################################################

all: $(ASM) #$(COREWAR)

$(ASM): $(LIBFT) $(ASM_OBJ)
	@printf "$(YELLOW)%-35s$(NC)" "Building $@... "
	@$(CC) $(CFLAGS) $(LIBFT) $(ASM_OBJ) -o $@
	@echo "$(GREEN)DONE$(NC)"

$(COREWAR): $(LIBFT) $(CORE_OBJ)
	@printf "$(YELLOW)%-35s$(NC)" "Building $@... "
	@$(CC) $(CFLAGS) $(LIBFT) $(CORE_OBJ) -o $@
	@echo "$(GREEN)DONE$(NC)"

$(LIBFT):
	@printf "$(YELLOW)%-35s$(NC)" "Building $@... "
	@make -C libft

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	@echo " > Compiling $<..."
	@$(CC) $(CFLAGS) $(INC) -c $< -o $@

clean:
	@rm -rf $(OBJ_DIR)
	@make -C libft clean
	@echo "$(RED)Object files removed$(NC)"

fclean: clean
	@make -C libft fclean
	@rm -f $(ASM)
	@echo "$(RED)$(ASM) removed"
	@rm -f $(COREWAR)
	@echo "$(COREWAR) removed$(NC)"

re: fclean all
