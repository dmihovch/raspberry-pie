all: scroll
scroll: main.o display.o input.o
	cc -o scroll main.o display.o input.o -L ~/lib -lsense -lm
run: scroll
	./scroll
clean:
	rm -f *.o scroll
display.o: display.c scroll.h
	cc -c display.c -L ~/lib -I ~/include -lsense
main.o: main.c scroll.h
	cc -c main.c -L ~/lib -I ~/include -lsense
input.o: input.c scroll.h
	cc -c input.c -L ~/lib -I ~/include -lsense
git:
	git add main.c display.c input.c Makefile
	@echo "Enter commit msg: "
	@read input && git commit -m "$$input"
	@echo "git push when ready!"

m:
	vim main.c
d:
	vim display.c
i:
	vim input.c


