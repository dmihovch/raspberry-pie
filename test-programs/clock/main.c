
#include "display.h"
#define MAX_BUF_SIZE 1024
#define BLACK 0x0000

int main(void){
	int open = open_display();
	if(open==0){
		printf("open_display failed or display already open\n");
		printf("Exiting...\n");
		exit(0);
	}
	char buf[MAX_BUF_SIZE];
	while(fgets(buf,MAX_BUF_SIZE,stdin)!= NULL){
		int hour; int minute; int second;
		if(sscanf(buf,"%2d:%2d:%2d",&hour,&minute,&second)==3){
			if(hour>=0 && hour<=23 && minute>=0 && minute<=59 && second>=0 && second<=59){
				display_time(hour,minute,second);
			}
			else{
				break;
			}
				
		}
		else{
			break;
		}
	}
	close_display();
	sleep(1);
	return 0;
}
