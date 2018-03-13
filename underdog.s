.name "underdog"
.comment "Eats mewbaccas for breakfast."
		
l0: aff r7
	ld %41, r7
l1:	sti	r1,%:live,%1
	ld %:live, r2
	ld %:l0, r3
live:	live	%1
	st r2,4
	st r1,4
	st r3,4
	zjmp	%:live
fork0:  live %1
		fork %:live
fork1:  live %1
		fork %:fork0
