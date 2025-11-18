	
.global _start
.text
_start:	
	#open frame buffer 0 
	mov x0,#0
	bl openfb
	cmp x0,#0
	blt .exit

	#get the color black and store it in x2
	mov x8,x0
	mov x0,#0
	mov x1,#0
	mov x2,#0	
	bl getColor
	mov x2,x0

	mov x13,#7
	#Nested for loops
.start_begin_outer_loop:
	cmp x13,#0
	b.lt .start_exit_outer_loop
	mov x12,#7
.start_begin_inner_loop:
	cmp x12,#0
	b.lt .start_exit_inner_loop
	#set pixel
	mov x0,x13
	mov x1,x12
	bl setPixel
	sub x12,x12,#1
	b .start_begin_inner_loop
.start_exit_inner_loop:
	sub x13,x13,#1
	b .start_begin_outer_loop
.start_exit_outer_loop:
	#cleanup the data structures created by openfb and close the framebuffer device
	bl closefb

.exit: 
	#clean exit
	mov x8,#94
	svc #0

