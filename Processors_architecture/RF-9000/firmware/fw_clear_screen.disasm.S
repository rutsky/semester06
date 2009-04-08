     0:	e3500000	cmp 	r0,	0x0
     4:	1a000001	bne	0x10
     8:	e59f302c	ldr 	r3, [pc, 0x2c] <- 0x200fea64
     c:	e5930000	ldr 	r0, [r3]
    10:	e8901008	ldmia	r0, {r3, ip}
    14:	e002039c	mul 	r2, ip, r3
    18:	e1a00000	mov 	r0, r0
    1c:	e2423001	sub 	r3,	r2,	0x1
    20:	e1a02003	mov 	r2, r3
    24:	e3730001	cmn 	r3,	0x1
    28:	0a000002	beq	0x38
    2c:	e0803082	add 	r3,	r0,	r2 lsl #1
    30:	e1c310b8	strh	r1, [r3, 0x8]
    34:	eafffff8	b	0x1c
    38:	e1a0f00e	mov 	pc, lr
    3c:	200fea64
