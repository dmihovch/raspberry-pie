.global _start
.text
_start:
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

        mov x12,#0
        mov x13,#0
        mov x2,x6
        mov x0,x12
        mov x1,x13
        bl setPixel
.main_loop:
        //x12 is x
        //x13 is y
        bl getJoystickValue
        mov x4,x0
        cmp x4,#0
        blt ._start_fbclose
        cmp x0,#5
        beq ._start_fbclose
        //remove the dot at x,y
        mov x2,#0
        mov x0,x12
        mov x1,x13
        bl setPixel
        
        cmp x4,#1 //up
        bne .skip1
        cmp x13,#0
        beq .draw
        sub x13,x13,#1
        mov x0,x12
        mov x1,x13
        mov x2,x6
        bl setPixel
        b .main_loop
.skip1:  cmp x4,#2       //right
        bne .skip2
        cmp x12,#7
        beq .draw
        add x12,x12,#1
        mov x0,x12
        mov x1,x13
        mov x2,x6
        bl setPixel
        b .main_loop
.skip2:  cmp x4,#3       //down
        bne .skip3
        cmp x13,#7
        beq .draw
        add x13,x13,#1
        mov x0,x12
        mov x1,x13
        mov x2,x6
        bl setPixel
        b .main_loop
.skip3:  cmp x4,#4       //left
        bne .main_loop
        cmp x12,#0
        beq .draw
        sub x12,x12,#1
        mov x0,x12
        mov x1,x13
        mov x2,x6
        bl setPixel
        b .main_loop
        
.draw:   mov x2,x6
        mov x0,x12
        mov x1,x13
        bl setPixel
        b .main_loop
        #cleanup the data structures created by openfb and close the framebuffer device
._start_fbclose:
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
