.global main
.text
main:
	bl openfb
	bl openJoystick


	mov x0,#2
	mov x1,#2
	mov x2,#0xFFFF
	bl setPixel
	
	
	bl sleepy
	

	bl closefb
	bl closeJoystick
	mov x8,#94
	svc #0
	ret
