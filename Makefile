CC=gcc
CFLAGS=-I./src
DEPS = ./src/fetchline.h ./src/file.h ./src/fetch.h
OBJ = ./src/main.o ./src/fetchline.o ./src/file.o ./src/fetch.o

colour_green=$(shell echo -e "\033[0;32m")
colour_reset=$(shell echo -e "\033[0m")

%.o: %.c $(DEPS)
	@$(CC) -c -o $@ $< $(CFLAGS)
	@echo "$(colour_green)CC $(colour_reset)$<"

cfetch: $(OBJ)
	@$(CC) -o $@ $^ $(CFLAGS)
	@echo "$(colour_green)CC $(colour_reset)$^"

.PHONY: clean

clean:
	@rm -f ./src/*.o ./cfetch
	@echo "Cleaned"
