NAME    := ws2txt
BIN_DIR := /usr/local/bin

all: build

build: $(NAME).c
	gcc -Wall -Wextra -o $(NAME) $<

test: $(NAME)
	./$(NAME) EXAMPLE.DOC EXAMPLE.TXT

install: $(NAME)
	install -m 0755 $< $(BIN_DIR)

uninstall:
	rm -f $(BIN_DIR)/$(NAME)

clean:
	rm -f $(NAME) EXAMPLE.TXT