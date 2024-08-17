ExecBase = 4

init:
	move.l	#"EXEC",d0
	jsr	print_4c
        move.l  ExecBase,a6
	move.l	a6,d0
	jsr	print_u32

	move.l	#"BLAH",d0
	jsr	print_4c
	move.l	#$cafed00d,d0
	jsr	print_u32

	moveq	#0,d0
	rts

	include	"lib_print.asm"
