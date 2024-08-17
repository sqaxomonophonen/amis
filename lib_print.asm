
print__doslib:	dc.b    "dos.library",0
print__buf:	dcb.b	10

print__begin:
.ExecBase = 4
.OpenLibrary = -552

; put "dos.library" handle in a5
        move.l  .ExecBase,a6
        lea     print__doslib,a1
        moveq   #0,d0
        jsr     .OpenLibrary(a6)
        tst.l   d0
        bne.s   .ok
	stop	#0
.ok:
	move.l	d0,a5
	rts

; number of characters in d3
print__end:
.CloseLibrary = -414
.Write = -48
.Output = -60
        jsr     .Output(a5)
        move.l  d0,d1
        move.l  #print__buf,d2
        jsr     .Write(a5)
	move.l	a5,a1
	jsr	.CloseLibrary(a6)
	rts


; print u32 value in d0 to Output()
print_u32:

.REGSAV	equrl	d1-d3/a0-a1/a5-a6

; save registers on stack
	movem.l	.REGSAV,-(a7)
	move.l	d0,d3
	jsr	print__begin
; render hex digits
.ndigits = 8
	lea	print__buf+.ndigits+1,a0 ; write buffer; writing in descending order
	lea	.hexdigits,a1 ; nybble->hex lut
	moveq	#10,d1 ; write \n
	move.b	d1,-(a0)
	moveq	#.ndigits-1,d2
	move.l	d3,d0
.hexloop:
	move.l	d0,d1
	andi	#15,d1
	move.b	(0,a1,d1.w),-(a0)
	lsr.l	#4,d0
	dbra	d2,.hexloop

	moveq	#.ndigits+1,d3
	jsr	print__end

	movem.l	(a7)+,.REGSAV
	rts
.hexdigits:	dc.b	"0123456789abcdef"

print_u32_end:
	

; print 4 characters in d0 to Output()
print_4c:
.REGSAV	equrl	d1-d3/a0-a1/a5-a6
	movem.l	.REGSAV,-(a7)
	move.l	d0,d3
	jsr	print__begin
	move.l	d3,d0
.nchars = 4
	lea	print__buf+.nchars+1,a0 ; write buffer; writing in descending order
	moveq	#10,d1 ; write \n
	move.b	d1,-(a0)
	moveq	#3,d2 ; 4 chars
.charloop:
	move.b	d0,-(a0)
	lsr.l	#8,d0
	dbra	d2,.charloop

	moveq	#.nchars+1,d3
	jsr	print__end

	movem.l	(a7)+,.REGSAV
	rts
print_4c_end:
