start	lw	0	1	one
	nor	0	1	2
	add	0	1	3
	beq	2	3	start
	halt

one	.fill	-1