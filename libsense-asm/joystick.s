# Exported
# 	int openJoystick(int deviceNum)
#	void closeJoystick()
#	int getJoystickValue()

.global openJoystick
.global closeJoystick
.global getJoystickValue

.data
.align 3
joyFileName: .asciz "/dev/input/event0"
joyFileDescriptor: .word 0
joyEvent: .zero 48

.text
.align 2
# int openJoystick(void)
#	returns a negative number on failure, file descriptor on success
openJoystick:
	ldr x1,=joyFileName
	# Open the file
	mov x0,#-100 
	ldr x1,=joyFileName
	mov x2, #2
	mov x8,#56
	svc #0
	cmp x0,#0
	blt .openJoystick_exit
	ldr x1, =joyFileDescriptor
	str x0,[x1]
.openJoystick_exit:
	ret

# void closeJoystick(void)
closeJoystick:
	ldr x0,=joyFileDescriptor
	ldr x0,[x0]
	mov x8,#57
	svc #0
	ret

# int getJoystickValue()
#	returns a numeric value 0,1,2,3,4,5 which mean
#		0: timeout occurred
#		1: up
#		2: right
#		3: down
#		4: left
#		5: click
getJoystickValue:
	//read 24 bytes from joystick
	ldr x0,=joyFileDescriptor
	ldr x0,[x0]
	ldr x1,=joyEvent
	mov x2,#48
	mov x8,#0x3F
	svc #0
	cmp x0,#0
	blt .getJoystickValue_error
	ldr x1,=joyEvent
	ldrh w0,[x1,#16] //type
	ldrh w1,[x1,#18] //code
	//type we want is 1
	cmp w0,#1
	bne getJoystickValue
	//code is left=105, right=106, up=103,down=108,click=28
	cmp w1,#105
	mov x0,#4
	beq .getJoystickValue_exit
	mov x0,#2
	cmp w1,#106
	beq .getJoystickValue_exit
	mov x0,#1
	cmp w1,#103
	beq .getJoystickValue_exit
	mov x0,#3
	cmp w1,#108
	beq .getJoystickValue_exit
	mov x0,#5
	cmp w1,#28
	beq .getJoystickValue_exit
	mov x0,#-1
.getJoystickValue_exit:
	ret
.getJoystickValue_error:
	mov x0,#-1
	ret
