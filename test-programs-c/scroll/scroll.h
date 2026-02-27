#include <stdio.h>
#include <stdlib.h>
#include "../../lib/piemulator.h"
#include <stdbool.h>
#include <string.h>
#define BLACK 0x0000
#define WHITE 0xFFFF
#include <stddef.h>
#include <unistd.h>
#include <linux/input.h>
#include <math.h>
void open_display(void);
void close_display(void);
void display_letter(char,int,int);
void clear_display(void);
void open_input(void);
void close_input(void);
void check_input(void (*callback)(unsigned int code),int); //if there is an issue, it's this
void handler(unsigned int);
float checkOffset(float);
