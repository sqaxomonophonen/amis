
init:
.ExecBase = 4
.AllocMem = -198
.MEMF_CHIP = 1<<1
.MEMF_FAST = 1<<2
.MEMF_LARGEST = 1<<17
        move.l  .ExecBase,a6

	move.l	#$10000,d0
	moveq	#.MEMF_CHIP,d1
        jsr     .AllocMem(a6)
	move.l	d0,a0
        tst.l	d0
	beq.s	err_exit

	moveq	#1,d0
	bsr	pwm

ALEN = 32

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
	move.w	#ALEN/2,.audLEN(a1)
	move.w	#123,.audPER(a1)
	move.w	#64,.audVOL(a1)
	move.w	#$8001,.audDMACON(a1)
	

; TODO arp?

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


pwm: ; d0=width, a0=dstptr, destroys d0-d2/a1
	move.l	a0,a1
	moveq	#ALEN-1,d2 ; samples per period - 1
	moveq	#$7f,d1
.loop:
	subq	#1,d0
	bcc.s	.skip
	addq	#1,d1 ; 127 => -128
.skip:
	move.b	d1,(a1)+
	dbra	d2,.loop

	rts

	include	"lib_print.asm"
