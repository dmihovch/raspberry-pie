#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "sense.h"

int open_display(void);
void display_time(int,int,int);
void display_colons(void);
void display_hours(int);
void display_minutes(int);
void display_seconds(int);
void close_display(void);
