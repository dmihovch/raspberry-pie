

all:
	gcc -o emul example-test.c src/piscreen.c -lncurses -lpthread

run:
	./bin/pie

clean:
	rm bin/pie

valgrind:
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./emul

gpt:
	gcc -o gpt chat-gpt-test.c src/piscreen.c -lncurses -lpthread -lm
