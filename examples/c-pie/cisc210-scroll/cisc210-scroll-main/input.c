#include "scroll.h"

pi_joystick_t *js = NULL;

void open_input(void){
	js = getJoystickDevice();	
}
void close_input(void){
	if(js != NULL){
		freeJoystick(js);
		js = NULL;
	}
}
void check_input(void (*callback)(unsigned int code),int delay){
	pollJoystick(js,callback,delay);
}

