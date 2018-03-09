.name "zork"
.comment "just a basic living prog"
		
l2:	sti	r1,%:live,%1
	and	r1,%0,r1
live:	live	%1
	fork	%:l2
	ldi	%8, %:l2, r15
	zjmp	%:live
