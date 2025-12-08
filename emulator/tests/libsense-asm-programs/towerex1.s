.global main
.text
main:
        #open joystick
        bl openJoystick
        cmp x0,#0
        blt ._start_exit

        #open frame buffer
        bl openfb
        cmp x0,#0
        blt ._start_exit
        mov x8,x0

        #get the color red and store it in x6
        mov x0,#31
        mov x1,#0
        mov x2,#0
        bl getColor
        mov x6,x0



	# setting x12 (y) and x13 (x) to 0
        mov x12,#7
        mov x13,#3
	# moving color and y and x values
        mov x2,x6
        mov x0,x12
        mov x1,x13
        bl setPixel
.main_loop:
        //x12 is y
        //x13 is x
        bl getJoystickValue

	cmp x12, #7
	b.gt .pass_clear
	cmp x12, #0
	b.lt .pass_clear
	cmp x13, #7
	b.gt .pass_clear
	cmp x13, #0
	b.lt .pass_clear
	bl clear

.pass_clear:
        mov x4,x0
	# if x4 is -1 (joystick pressed in), exit
        cmp x4,#0
        blt ._start_fbclose

        cmp x0,#5
        #beq ._start_fbclose
	b.eq .change_color

        //remove the dot at x,y
        mov x2,#0
        mov x0,x12
        mov x1,x13
        bl setPixel
	b .pass_change_color

.change_color:
	asr x6, x6, #1

	mov x0, x12
	mov x1, x13
	mov x2, x6
	bl drawTower

	cmp x6, #4
	b.le ._start_fbclose
	b .draw

.pass_change_color:
        cmp x4,#1 //up
        bne .skip1
        cmp x13,#0
        beq .draw
        sub x13,x13,#1
        mov x0,x12
        mov x1,x13
        mov x2,x6
        bl setPixel

	mov x0, x12
	mov x1, x13
	mov x2, x6
	bl drawTower

        b .main_loop

.skip1:
	cmp x4,#2       //right
        bne .skip2
        cmp x12,#7
        beq .draw
        add x12,x12,#1
        mov x0,x12
        mov x1,x13
        mov x2,x6
        bl setPixel

	mov x0, x12
	mov x1, x13
	mov x2, x6
	bl drawTower

        b .main_loop

.skip2:
	cmp x4,#3       //down
        bne .skip3
        cmp x13,#7
        beq .draw
        add x13,x13,#1
        mov x0,x12
        mov x1,x13
        mov x2,x6
        bl setPixel

	mov x0, x12
	mov x1, x13
	mov x2, x6
	bl drawTower

        b .main_loop

.skip3:
	cmp x4,#4       //left
        bne .main_loop
        cmp x12,#0
        beq .draw
        sub x12,x12,#1
        mov x0,x12
        mov x1,x13
        mov x2,x6
        bl setPixel

	mov x0, x12
	mov x1, x13
	mov x2, x6
	bl drawTower

        b .main_loop

.draw:
	mov x2,x6
        mov x0,x12
        mov x1,x13
        bl setPixel

	mov x0, x12
	mov x1, x13
	mov x2, x6
	bl drawTower

        b .main_loop

#------------------------------------------------------------------------

drawTower:
	#x0 has y, x1 has x and x2 has color
	# keep moving the dot down and adding to the ends

	# store important stuff
	sub sp, sp, #64
	str lr, [sp, #0]
	str x3, [sp, #8]
	str x4, [sp, #16]
	str x5, [sp, #24]

	# set bounds
	# upper bounds
	mov x3, x1
	# lower bounds
	mov x4, x1

.tower_outer_loop:
	cmp x0, #7
	b.eq .tower_end

	# move tower down a line
	add x0, x0, #1
	add x3, x3, #1
	sub x4, x4, #1

	# see if tower is still in bounds
	cmp x3, #7
	b.gt .keep_in_upper_bound

	#.keep_in_lower_bound
	cmp x4, #0
	b.lt .keep_in_lower_bound
	b .tower_inner_loop0

.keep_in_upper_bound:
	sub x3, x3, #1
	cmp x4, #0
	b.lt .keep_in_lower_bound
	b .tower_inner_loop0

.keep_in_lower_bound:
	add x4, x4, #1
	b .tower_inner_loop0

.tower_inner_loop0:
	mov x5, x3
.tower_inner_loop:
	# x5 will be the shifting dot
	cmp x5, x4
	b.lt .tower_outer_loop

	str x0, [sp, #32]
	str x1, [sp, #40]
	str x2, [sp, #48]
	str x3, [sp, #56]

	mov x0, x0
	mov x1, x5
	mov x2, x2
	bl setPixel

	ldr x0, [sp, #32]
	ldr x1, [sp, #40]
	ldr x2, [sp, #48]
	ldr x3, [sp, #56]

	sub x5, x5, #1
	b .tower_inner_loop

.tower_end:
	ldr lr, [sp, #0]
	ldr x3, [sp, #8]
	ldr x4, [sp, #16]
	ldr x5, [sp, #24]
	add sp, sp, #64
	ret

clear:
	sub sp, sp, #48
	str lr, [sp, #0]
	str x0, [sp, #8]
	str x1, [sp, #16]
	str x2, [sp, #24]
	str x8, [sp, #32]
	str x12, [sp, #40]
	str x13, [sp, #48]

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
	ldr lr, [sp, #0]
	ldr x0, [sp, #8]
	ldr x1, [sp, #16]
	ldr x2, [sp, #24]
	ldr x8, [sp, #32]
	ldr x12, [sp, #40]
	ldr x13, [sp, #48]
	add sp, sp, #48
	ret

#------------------------------------------------------------------------

        #cleanup the data structures created by openfb and close the framebuffer device
._start_fbclose:
	bl clear
        bl clearScreen
        bl closefb

._start_exit:
        #close joystick device
        bl closeJoystick
        cmp x0,#0
        blt ._start_exit

        #clean exit
        mov x8,#94
        svc #0
clearScreen:
        sub sp,sp,#16
        str lr,[sp]
        mov x11,#0
clearScreen_loop:
        cmp x11,#48
        bge clearScreen_exit
        mov x0,x11
        mov x1,#8
        udiv x2, x0, x1
        msub x3, x2, x1, x0
        // result: x2=quotient, x3=remainder
        mov x1,x2
        mov x0,x3
        mov x2,#0
        bl setPixel
        add x11,x11,#1
        b clearScreen_loop
clearScreen_exit:

        ldr lr,[sp]
        add sp,sp,#16
        ret
