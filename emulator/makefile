

all:
	gcc -o emul example-test.c src/piscreen.c -lncurses -lpthread

run:
	./emul

clean:
	rm emul

valgrind:
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./emul
