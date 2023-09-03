GIT_VERSION := "$(shell git describe --abbrev=4 --dirty --always --tags)"
CC	  := gcc
LDFLAGS := -rdynamic -pg \
		   -fsanitize=address
CFLAGS  := -Wall -Wextra \
		   -O3 -march=native \
		   -rdynamic -pg \
		   -fsanitize=address

SRC_DIR := .
OBJ_DIR := obj
BIN_DIR := bin

CSUFFIX := .c
C_FILES := $(wildcard *$(CSUFFIX))
O_FILES := $(addprefix $(OBJ_DIR)/,$(notdir $(C_FILES:$(CSUFFIX)=.o)))
D_FILES := $(addprefix $(OBJ_DIR)/,$(notdir $(C_FILES:$(CSUFFIX)=.d)))
JOBS := example_person example_int
TARGETS := $(addprefix $(BIN_DIR)/,$(JOBS))

.phony: all clean list bin obj $(JOBS)

all: $(TARGETS)

bin:
	mkdir $(BIN_DIR)
obj:
	mkdir $(OBJ_DIR)
list:
	@echo $(JOBS)

# depend on include files
-include $(D_FILES)

define link
	@echo link	: $@ $^
	@$(CC) $(LDFLAGS) -o $@ $^
endef

# compile all .c Files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%$(CSUFFIX) Makefile | $(OBJ_DIR)
	@echo compile : $<
	@$(CC) $(CFLAGS) -c -DVERSION=\"$(GIT_VERSION)\" -MMD -MP -o $@ $<

################################################################################
# EXAMPLES #####################################################################
################################################################################

EX_FILES := example_person.o lutd_person.o person.o
$(BIN_DIR)/example_person: $(addprefix $(OBJ_DIR)/,$(EX_FILES)) | $(BIN_DIR)
	$(call link $@ $^)

EX_FILES := example_int.o lutd_int.o
$(BIN_DIR)/example_int: $(addprefix $(OBJ_DIR)/,$(EX_FILES)) | $(BIN_DIR)
	$(call link $@ $^)

################################################################################
### Cleaning ###################################################################
################################################################################

ifeq ($(OS),Windows_NT)
# Cleaning rules for Windows OS
clean:
	@del /q $(OBJ_DIR), $(TARGETS), $(BIN_DIR)
	@rmdir $(OBJ_DIR)
	@rmdir $(BIN_DIR)
else
# Cleaning rules for Unix-based OS (no clue if this works)
clean:
	@rm -rf $(OBJ_DIR) $(TARGETS) $(BIN_DIR)
endif

