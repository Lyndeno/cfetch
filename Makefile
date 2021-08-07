CC=gcc
CFLAGS=-I./src
DEPS = ./src/fetchline.h ./src/file.h ./src/fetch.h
OBJ = ./src/main.o ./src/fetchline.o ./src/file.o ./src/fetch.o

%.o: %.c $(DEPS)
	@$(CC) -c -o $@ $< $(CFLAGS)
	@echo "CC $<"

cfetch: $(OBJ)
	@$(CC) -o $@ $^ $(CFLAGS)
	@echo "CC $^"

.PHONY: clean

clean:
	@rm -f ./src/*.o ./cfetch
	@echo "Cleaned"
