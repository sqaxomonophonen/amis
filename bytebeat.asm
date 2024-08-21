
init:
.ExecBase = 4
.AllocMem = -198
.MEMF_CHIP = 1<<1
.MEMF_FAST = 1<<2
.MEMF_LARGEST = 1<<17
        move.l  .ExecBase,a6

BUFSIZE=$d000

	move.l	#BUFSIZE,d0
	moveq	#.MEMF_CHIP,d1
        jsr     .AllocMem(a6)
	move.l	d0,a0
        tst.l	d0
	beq	err_exit

	move.l	#"REND",d0
	jsr	print_4c


	move.l	#BUFSIZE-1,d0
	moveq	#0,d1
	moveq	#0,d2
	move.l	#77,d3
	move.l	#1064,d4
	move.l	a0,a1
	; like in playground0.c:
	; this is expensive.. takes ~2 seconds? :-(
	; the shifts could probably be optimized a bit
.rend_loop:

	move.l	d1,d5
	lsl.l	#5,d5
	and.l	d1,d5
	beq.s	.c1
	move.l	d1,d5
	;lsr.l	#13,d5
	lsr.l	#7,d5
	lsr.l	#6,d5
	bra.s	.cx
.c1:
	move.l	d1,d5
	lsr.l	#2,d5
.cx:
	
	move.l	d1,d6
	lsr.l	#7,d6
	lsr.l	#2,d6
	andi	#7,d6
	move.l	d2,d7
	lsr.l	#7,d7
	and.l	d7,d6
	move.l	d1,d7
	lsr.l	#7,d7
	lsr.l	#5,d7
	sub.l	d7,d6
	or.l	d6,d5

	move.b	d5,(a1)+

	add.l	d3,d1
	add.l	d4,d2
	dbra	d0,.rend_loop

	move.l	#"PLAY",d0
	jsr	print_4c

; play waveform
play:
.AUD0 = $dff0a0
.audLCH = 0
.audLEN = 4
.audPER = 6
.audVOL = 8
.audDMACON = -10
	lea	.AUD0,a1
	move.l	a0,.audLCH(a1)
	move.w	#BUFSIZE/2,.audLEN(a1) ; number of words, not bytes
	move.w	#123,.audPER(a1)
	move.w	#64,.audVOL(a1)
	move.w	#$8001,.audDMACON(a1)

ok_exit:
	move.l	#"OK  ",d0
	jsr	print_4c
	bra	exit

err_exit:
	move.l	#"ERR!",d0
	jsr	print_4c
	bra	exit

exit:
	moveq	#0,d0
	rts

	include	"lib_print.asm"

