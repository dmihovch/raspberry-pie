.data
	FILENAME: .asciz "/dev/fb?"
	FD: .word 0
	FB: .word 0
	
.global openfb
.global closefb
.global getColor
.global setPixel
.text
#openfb(fb#) returns a pointer to a memory mapped 8x8x16 grid.
openfb:	ldr x1,=FILENAME
	# Try first to open /dev/fb1
	# 49 is ascii for '1'
	mov w0,#49
	strb w0,[x1,#7]
	mov x2,#2
	mov x0,#-100
	mov x8,#56
	svc #0
	cmp x0,#0
	bge .openfb_continue
	# Failed. Try to open /dev/fb0
	# 48 is ascii for '0'
	ldr x1,=FILENAME
	mov w0,#48
	strb w0,[x1,#7]
	svc #0
	cmp x0,#0
	blt .openfb_exit
.openfb_continue:
	ldr x1,=FD
	str x0,[x1,#0]
	mov x5,#0
	mov x4,x0
	mov x3,#1
	mov x2,#3
	mov x1,#128
	mov x0,#0
	mov x8,#222
	svc #0
	ldr x1,=FB
	str x0,[x1,#0]
.openfb_exit:
	ret
	
#closefb() Unmaps file and closes it.  Returns the status code for closing the file
closefb:
	ldr x0,=FB
	mov x1,#128
	mov x2,#4 //Set flag to synchronous
	mov x8,#227
	svc #0
	ldr x0,=FB
	mov x1,#128
	mov x8,#215
	svc #0
	ldr x3,=FD
	ldr x0,[x3]
	mov x7,#57
	svc #0
	ret

#uint_16 getColor(r,g,b) gets a valid color code from r,g,b values between 0-31
getColor:
	mov x4,x0
	lsl x4,x4,#6
	add x1,x1,x1
	add x4,x4,x1
	lsl x4,x4,#5
	add x4,x4,x2
	mov x0,x4
	ret
	
#setPixel(x,y,color) where x,y 0-7 and color returned by getcolor.  Turns on that light
setPixel:
	ldr x3,=FB
	lsl x1,x1,#4
	lsl x0,x0,#1
	add x1,x1,x0
	ldr x3,[x3,#0]
	strh w2,[x3,x1]
	#Going to add a call to msync.to flush.
	ret
