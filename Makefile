SRC_DIR := src
OBJ_DIR := obj
BIN_DIR := .

EXE := $(BIN_DIR)/cfetch
SRC := $(wildcard $(SRC_DIR)/*.c)
OBJ := $(SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

CPPFLAGS := -Iinc -MMD -MP
CFLAGS := -Wall
LDFLAGS := -Llib
LDLIBS :=

colour_green=$(shell echo -e "\033[0;32m")
colour_reset=$(shell echo -e "\033[0m")

.PHONY: all clean

all: $(EXE)

$(EXE): $(OBJ) | $(BIN_DIR)
	@$(CC) $(LDFLAGS) $^ $(LDLIBS) -o $@
	@echo "$(colour_green)CC $(colour_reset)$^ $@"

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	@$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@
	@echo "$(colour_green)CC $(colour_reset)$< $@"

$(BIN_DIR) $(OBJ_DIR):
	@mkdir -p $@

clean:
	@$(RM) -rv $(EXE) $(OBJ_DIR)

-include $(OBJ:.o=.d)
