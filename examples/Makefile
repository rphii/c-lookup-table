ifeq ($(OS),Windows_NT)
ifeq ($(XSUFFIX),)
	XSUFFIX := .exe
endif
endif

GIT_VERSION := "$(shell git describe --abbrev=4 --dirty --always --tags)"
CC	  := gcc
LDFLAGS := #-rdynamic -pg \
		   #-fsanitize=address
CFLAGS  := -Wall -Wextra \
		   -O3 -march=native -Werror \
		   #-rdynamic -pg \
		   #-fsanitize=address

BIN_DIR := bin
OBJ_DIR := obj
CSUFFIX := .c
JOBS := example_person example_int
TARGETS := $(addprefix $(BIN_DIR)/,$(addsuffix $(XSUFFIX),$(JOBS)))

.phony: all clean list bin obj $(JOBS) $(TARGETS)


### LINKING FUNCTION : $(call link,LDFLAGS,EXECUTABLE,PATH/TO/FILE.o PATH/TO/ANOTHERFILE.o)
# arguments
# 1: [IN]  linker options (LDFLAGS)
# 2: [OUT] executable output
# 3: [IN]  path/to/file.o path/to/anotherfile.o
define link
	@echo link	: $@ $^
	@$(CC) $1 -o $2 $3
endef

### COMPILATION FUNCTION : $(call compile,CFLAGS,OBJ_DIR,PATH/FILE.o)
# arguments
# 1: [IN]  compile options (CFLAGS)
# 2: [OUT] object directory
# 3: [IN]  path/to/file.c
define compile
	@echo compile : $3
	@$(CC) $1 -DVERSION=\"$(GIT_VERSION)\" -c -MMD -MP -o $(addprefix $2/,$(notdir $(basename $3)).o) $3
endef

### CREATE BINARY FUNCTION : $(call CFLAGS,LDFLAGS,OBJ_DIR,BIN_PATH,PATH/TO/FILE.c PATH/TO/ANOTHERFILE.c)
# arguments
# 1: [IN]  compile options
# 2: [IN]  linker options
# 3: [IN]  object directory
# 4: [OUT] binary path
# 5: [IN]  source files
define binary
BIN_DIR := $$(dir $4)
BIN_DIR := $$(BIN_DIR:/=)
OBJ_DIR := $3
C_FILES := $5
O_FILES := $$(addprefix $$(OBJ_DIR)/,$$(addsuffix .o,$$(notdir $$(basename $$(C_FILES)))))
D_FILES := $$(addprefix $$(OBJ_DIR)/,$$(addsuffix .d,$$(notdir $$(basename $$(C_FILES)))))

-include $$(D_FILES)

# compile any .c files
$$(OBJ_DIR)/%.o: %.c Makefile | $$(OBJ_DIR)
	$$(call compile,$1,$$(OBJ_DIR),$$<)

# link executable
$4$$(XSUFFIX): $$(O_FILES) | $$(BIN_DIR)
	$$(call link,$2,$$@,$$^)

endef

all: $(TARGETS)

$(eval $(call binary,$(CFLAGS),$(LDFLAGS),$(OBJ_DIR),bin/example_person,example_person.c lutd_person.c person.c))
$(eval $(call binary,$(CFLAGS),$(LDFLAGS),$(OBJ_DIR),bin/example_int,example_int.c lutd_int.c))

$(BIN_DIR):
	mkdir $(BIN_DIR)

$(OBJ_DIR):
	mkdir $(OBJ_DIR)

list:
	@echo possible binaries
	@$(foreach TARGET,$(TARGETS),echo "- $(TARGET)";)

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

