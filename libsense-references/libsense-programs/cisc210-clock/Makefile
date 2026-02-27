
all: clockdisplay
clockdisplay: main.o display.o
	cc -o clockdisplay main.o display.o -L ~/lib -lsense -lm

clean:
	rm -f *.o clockdisplay
main.o: main.c display.h
	cc -c main.c -L ~/lib -I ~/include -lsense
display.o: display.c display.h
	cc -c display.c -L ~/lib -I ~/include -lsense
run:
	./clock.sh | ./clockdisplay
