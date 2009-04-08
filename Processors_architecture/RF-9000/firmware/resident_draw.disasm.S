     0:	e92d4fff	stmdb	sp!, {r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, sl, fp, lr}
     4:	e24dd000	sub 	sp,	sp,	0x0
     8:	e59f5074	ldr 	r5, [pc, 0x74] <- 0x200fea64
     c:	e3a06064	mov 	r6, 0x64
    10:	e3a07064	mov 	r7, 0x64
    14:	e2855008	add 	r5,	r5,	0x8
    18:	e1a03005	mov 	r3, r5
    1c:	e3a00000	mov 	r0, 0x0
    20:	e3a01000	mov 	r1, 0x0
    24:	e0040690	mul 	r4, r0, r6
    28:	e0844001	add 	r4,	r4,	r1
    2c:	e59f4078	ldr 	r4, [pc, 0x78] <- 0xaaaa
    30:	e1c340b0	strh	r4, [r3, 0x0]
    34:	e2833002	add 	r3,	r3,	0x2
    38:	e2811001	add 	r1,	r1,	0x1
    3c:	e1510006	cmp 	r1,	r6
    40:	bafffff7	blt	0x24
    44:	e2800001	add 	r0,	r0,	0x1
    48:	e1500007	cmp 	r0,	r7
    4c:	bafffff3	blt	0x20
    50:	e92d00ff	stmdb	sp!, {r0, r1, r2, r3, r4, r5, r6, r7}
    54:	e1a0e00f	mov 	lr, pc
    58:	e59ff028	ldr 	pc, [pc, 0x28] <- 0x200330fc
    5c:	e1a09000	mov 	r9, r0
    60:	e3a00001	mov 	r0, 0x1
    64:	e1a0e00f	mov 	lr, pc
    68:	e59ff01c	ldr 	pc, [pc, 0x1c] <- 0x200a06c0
    6c:	e8bd00ff	ldmia	sp!, {r0, r1, r2, r3, r4, r5, r6, r7}
    70:	e3590000	cmp 	r9,	0x0
    74:	1a000000	bne	0x7c
    78:	eaffffe6	b	0x18
    7c:	e28dd000	add 	sp,	sp,	0x0
    80:	e8bd8fff	ldmia	sp!, {r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, sl, fp, pc}
    84:	200fea64
    88:	200330fc
    8c:	200a06c0
    90:	20054684	andcs 	r4,	r5,	r4 lsl #13
    94:	0000f81f
    98:	0000fe5b
    9c:	6c6c6548	copvs
    a0:	0000216f
    a4:	74736554	ldrvcb	error 'write back' set
    a8:	00000000
    ac:	0000aaaa
