CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -g -fsanitize=address
OBJ_DIR = build
SRC_DIR = .
BIN = vm

SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRCS))

all: $(BIN)

$(BIN): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

run:
	@make
	@./$(BIN)

clean:
	rm -rf $(BIN) $(OBJ_DIR)

rebuild: clean all

DEPFILES = $(OBJS:.o=.d)
-include $(DEPFILES)

$(OBJ_DIR)/%.d: $(SRC_DIR)/%.c | $(OBJ_DIR)
	@$(CC) $(CFLAGS) -MM $< -MT $(OBJ_DIR)/$*.o -o $@

.PHONY: all clean rebuild


