CFLAGS=-Wall -Werror

.PHONY: clean

run: tema0
	@./check.sh

tema0: tema0.c
	gcc tema0.c $(CFLAGS) -o tema0

archive:
	zip -r homework0.zip \
	Makefile README \
	*.c *.h

clean:
	rm -f tema0
