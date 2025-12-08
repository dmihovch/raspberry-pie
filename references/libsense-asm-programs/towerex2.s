.global _start
.text

_start:
    bl openfb
    cmp x0, #0
    blt exit

    mov x20, #4
    mov x21, #7

    mov x22, #0
    mov x23, #0

    bl openJoystick
    cmp x0, #0
    blt exit

main_loop:
    bl draw_tower
    bl getJoystickValue
    cmp x0, #5
    b.ne check_up

    add x22, x22, #1
    add x23, x23, #1

    cmp x23, #10
    b.ge cleanup

    mov x2, #10000
button_delay:
    subs x2, x2, #1
    b.gt button_delay
    b main_loop

check_up:
    cmp x0, #1
    b.ne check_right
    cmp x21, #0
    b.le main_loop
    sub x21, x21, #1
    b movement_delay

check_right:
    cmp x0, #2
    b.ne check_down
    cmp x20, #7
    b.ge main_loop
    add x20, x20, #1
    b movement_delay

check_down:
    cmp x0, #3
    b.ne check_left
    cmp x21, #7
    b.ge main_loop
    add x21, x21, #1
    b movement_delay

check_left:
    cmp x0, #4
    b.ne main_loop
    cmp x20, #0
    b.le main_loop
    sub x20, x20, #1

movement_delay:
    mov x2, #5000
delay_loop:
    subs x2, x2, #1
    b.gt delay_loop
    b main_loop

draw_tower:
    stp x29, x30, [sp, #-16]!
    stp x19, x20, [sp, #-16]!
    stp x21, x22, [sp, #-16]!

    mov x0, #0
    mov x1, #0
    mov x2, #0
    bl getColor
    mov x19, x0

    mov x10, #0
clear_y:
    mov x11, #0
clear:
    mov x0, x11
    mov x1, x10
    mov x2, x19
    bl setPixel
    add x11, x11, #1
    cmp x11, #8
    b.lt clear
    add x10, x10, #1
    cmp x10, #8
    b.lt clear_y

    mov x0, x22
    bl get_current_color
    mov x19, x0

    mov x0, x20
    mov x1, x21
    mov x2, x19
    bl setPixel

    cmp x21, #7
    b.ge draw_done

    mov x10, x21
    add x10, x10, #1

triangle_row_loop:
    cmp x10, #8
    b.ge draw_done

    sub x11, x10, x21
    sub x11, x11, #1
    lsl x11, x11, #1
    add x11, x11, #3

    lsr x12, x11, #1
    sub x12, x20, x12

    mov x13, #0
draw_row_pixels:
    cmp x13, x11
    b.ge next_row

    add x14, x12, x13

    cmp x14, #0
    b.lt skip_pixel
    cmp x14, #7
    b.gt skip_pixel

    mov x0, x14
    mov x1, x10
    mov x2, x19
    bl setPixel

skip_pixel:
    add x13, x13, #1
    b draw_row_pixels

next_row:
    add x10, x10, #1
    b triangle_row_loop

draw_done:
    ldp x21, x22, [sp], #16
    ldp x19, x20, [sp], #16
    ldp x29, x30, [sp], #16
    ret

get_current_color:
    stp x29, x30, [sp, #-16]!

    and x0, x0, #7

    cmp x0, #0
    b.ne color_1
    mov x0, #31
    mov x1, #0
    mov x2, #0
    b make_color

color_1:
    cmp x0, #1
    b.ne color_2
    mov x0, #0
    mov x1, #63
    mov x2, #0
    b make_color

color_2:
    cmp x0, #2
    b.ne color_3
    mov x0, #0
    mov x1, #0
    mov x2, #31
    b make_color

color_3:
    cmp x0, #3
    b.ne color_4
    mov x0, #31
    mov x1, #63
    mov x2, #0
    b make_color

color_4:
    cmp x0, #4
    b.ne color_5
    mov x0, #31
    mov x1, #0
    mov x2, #31
    b make_color

color_5:
    cmp x0, #5
    b.ne color_6
    mov x0, #0
    mov x1, #63
    mov x2, #31
    b make_color

color_6:
    cmp x0, #6
    b.ne color_7
    mov x0, #31
    mov x1, #63
    mov x2, #31
    b make_color

color_7:
    mov x0, #31
    mov x1, #32
    mov x2, #0

make_color:
    bl getColor
    ldp x29, x30, [sp], #16
    ret

cleanup:
    mov x0, #0
    mov x1, #0
    mov x2, #0
    bl getColor
    mov x19, x0

    mov x10, #0
final_clear_y:
    mov x11, #0
final_clear:
    mov x0, x11
    mov x1, x10
    mov x2, x19
    bl setPixel
    add x11, x11, #1
    cmp x11, #8
    b.lt final_clear
    add x10, x10, #1
    cmp x10, #8
    b.lt final_clear_y

    bl closeJoystick
    bl closefb

exit:
    mov x8, #94
    svc #0