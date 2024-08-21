

; cheap highpass filter; a FIR with coefficiens=[-1,1]
; d0=n samples, a0=src, a1=dst
cheap_highpass:
	subq	#1,d0
	move.b	(a0)+,d1
.loop:
	move.b	(a0)+,d2
	move.b	d1,d3
	sub.w	d2,d3
	move.b	d3,(a1)+
	move.b	d2,d1
	dbra	d0,.loop


