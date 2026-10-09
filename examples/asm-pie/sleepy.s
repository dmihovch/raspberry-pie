// 1-second delay used by the example programs. Stands in for the sleep
// function provided by the course library. Uses the aarch64 nanosleep
// syscall directly so it depends on no libc call.
.global sleepy
.text
sleepy:
	sub sp, sp, #16
	mov x8, #101        // SYS_nanosleep on aarch64
	mov x0, sp          // timespec* requested
	mov x1, #1
	str x1, [sp]        // tv_sec = 1
	str xzr, [sp, #8]   // tv_nsec = 0
	mov x1, #0          // timespec* remaining = NULL
	svc #0
	add sp, sp, #16
	ret
