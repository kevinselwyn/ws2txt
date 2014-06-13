NAME   := ws2txt
BINDIR := /usr/local/bin

all: $(NAME)

$(NAME): $(NAME).c
	gcc -o $@ $<

test: $(NAME)
	./$(NAME) EXAMPLE.DOC EXAMPLE.TXT

memcheck:
	gcc $(NAME).c -o $(NAME) -O0 -g
	valgrind $(NAME)

install: $(NAME)
	install -m 0755 $< $(BINDIR)

uninstall:
	rm -f $(BINDIR)/$(NAME)

clean:
	rm $(NAME) EXAMPLE.TXT
