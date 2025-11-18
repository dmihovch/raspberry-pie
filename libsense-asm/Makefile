all: dots clear dotmove

dots: lights.o dots.o
	ld $^ -o $@
clear: lights.o clear.o
	ld $^ -o $@
dotmove: lights.o joystick.o dotmove.o
	ld $^ -o $@
lights.o: lights.s
	as -g $^ -o $@
joystick.o: joystick.s
	as -g $^ -o $@
clear.o: clear.s
	as -g $^ -o $@
dotmove.o: dotmove.s
	as -g $^ -o $@
clean:
	rm -f *.o example clear dotmove dots
