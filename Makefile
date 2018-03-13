#!/usr/bin/make -f

################################################################################
# SETTINGS                                                                     #
################################################################################

ASM = asm
COREWAR = corewar
CC = gcc
CFLAGS += -Wall -Werror -Wextra
LIBFT = libft/libft.a
ASM_DIR = assembler
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
CORE_DIR = vm
CORE_SRC = \
	arena\
	fetcher/mask_fetch\
	invalid\
	main\
	op\
	ops/op_add\
	ops/op_aff\
	ops/op_and\
	ops/op_fork\
	ops/op_ld\
	ops/op_ldi\
	ops/op_lfork\
	ops/op_live\
	ops/op_lld\
	ops/op_lldi\
	ops/op_or\
	ops/op_st\
	ops/op_sti\
	ops/op_sub\
	ops/op_xor\
	ops/op_zjmp\
	read_champion\
	utils\
	war/init\
	war/war_op
ASM_OBJ = $(patsubst %, $(ASM_DIR)/obj/%.o, $(ASM_SRC))
CORE_OBJ = $(patsubst %, $(CORE_DIR)/obj/%.o, $(CORE_SRC))
INC = -I libft/inc
ASM_INC = -I $(ASM_DIR)/inc
CORE_INC = -I $(CORE_DIR)/includes

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

all: $(ASM) $(COREWAR)

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

$(ASM_DIR)/obj/%.o: $(ASM_DIR)/src/%.c
	@mkdir -p $(dir $@)
	@echo " > Compiling $<..."
	@$(CC) $(CFLAGS) $(INC) $(ASM_INC) -c $< -o $@

$(CORE_DIR)/obj/%.o: $(CORE_DIR)/%.c
	@mkdir -p $(dir $@)
	@echo " > Compiling $<..."
	@$(CC) $(CFLAGS) $(INC) $(CORE_INC) -c $< -o $@

clean:
	@rm -rf $(ASM_DIR)/obj
	@rm -rf $(CORE_DIR)/obj
	@make -C libft clean
	@echo "$(RED)Object files removed$(NC)"

fclean: clean
	@make -C libft fclean
	@rm -f $(ASM)
	@echo "$(RED)$(ASM) removed"
	@rm -f $(COREWAR)
	@echo "$(COREWAR) removed$(NC)"

re: fclean all
