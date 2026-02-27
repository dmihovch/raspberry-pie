	
.global _start
.text
_start:	
	# Open frame buffer and validate
	bl openfb
	cmp x0,#0
	blt ._start_exit
	mov x8,x0

	#get the color blue and store it in x6
	mov x0,#0
	mov x1,#0
	mov x2,#31	
	bl getColor
	mov x6,x0

	#set pixel 4,7 to the blue color stored in x6 
	mov x2,x6
	mov x0,#4
	mov x1,#7
	bl setPixel
	#set pixel 0,0 to the blue color stored in x6
	mov x2,x6
	mov x0,#0
	mov x1,#0
	bl setPixel

	#cleanup the data structures created by openfb and close the framebuffer device
	bl closefb

._start_exit: 
	#clean exit
	mov x8,#94
	svc #0

