#include "scroll.h"

int vX = 0;
int vY = 0;

bool running = true;

int count = 0; //to change letter

char currentLetter = 'd';

int main(void){
	open_display();
	open_input();
	while(running){
		check_input(handler,100);
		display_letter(currentLetter,vX,vY);
	}
	clear_display();
	close_input();
	close_display();
	return 0;
}

void handler(unsigned int code){
	//fprintf(stderr,"code: %u\t",code);
	if(code == KEY_UP){//left
		//fprintf(stderr,"left\t");
		if(vX > -10){
			vX--;
			//fprintf(stderr,"vX decreased by 1");
		}
	}
	else if(code == KEY_DOWN){//right
		//fprintf(stderr,"right\t");
		if(vX < 10){
			vX++;
			//fprintf(stderr,"vX increased by 1");
		}
	}
	else if(code == KEY_RIGHT){//up
		//fprintf(stderr,"up\t");
		if(vY < 10){
			vY++;
			//fprintf(stderr,"vY increased by 1");
		}
	}
	else if(code == KEY_LEFT){//down
		//fprintf(stderr,"down\t");
		if(vY > -10){
			vY--;
			//fprintf(stderr,"vY decreased by 1");
		}
	}
	else if(code == KEY_ENTER){
		//fprintf(stderr,"push\t");
		count++;
		if(count == 1){
			//fprintf(stderr,"change letter to M");
			currentLetter = 'm';
			vY = 0;
			vX = 0;
		}
		if(count == 2){
			//fprintf(stderr,"running = false now");
			running = false;
		}
	}
	//fprintf(stderr,"\n");
}
