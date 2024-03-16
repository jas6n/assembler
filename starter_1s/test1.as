start	lw	0	0	one
	nor	0	1	2
	add	0	1	3
	beq	1	0	start
	halt
one	.fill	-1
	.fill 	1
	.fill	9
	.fill	2