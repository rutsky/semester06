     0:	e92d4fff	stmdb	sp!, {r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, sl, fp, lr}
     4:	e24dd020	sub 	sp,	sp,	0x20
     8:	e3a00000	mov 	r0, 0x0
     c:	e59f10e0	ldr 	r1, [pc, 0xe0] <- 0x0
    10:	e1a0e00f	mov 	lr, pc
    14:	e59ff0b8	ldr 	pc, [pc, 0xb8] <- 0x2004fad0
    18:	e59f50a0	ldr 	r5, [pc, 0xa0] <- 0x200fea64
    1c:	e3a06064	mov 	r6, 0x64
    20:	e3a07064	mov 	r7, 0x64
    24:	e2855008	add 	r5,	r5,	0x8
    28:	e59f00c8	ldr 	r0, [pc, 0xc8] <- 0xffff
    2c:	e59f10c0	ldr 	r1, [pc, 0xc0] <- 0x0
    30:	e1c500b0	strh	r0, [r5, 0x0]
    34:	e1c510b2	strh	r1, [r5, 0x2]
    38:	e1c500b4	strh	r0, [r5, 0x4]
    3c:	e1c510b6	strh	r1, [r5, 0x6]
    40:	e1c500b8	strh	r0, [r5, 0x8]
    44:	e1c510ba	strh	r1, [r5, 0xa]
    48:	e1c500bc	strh	r0, [r5, 0xc]
    4c:	e1c510be	strh	r1, [r5, 0xe]
    50:	e1a03005	mov 	r3, r5
    54:	e3a00000	mov 	r0, 0x0
    58:	e3a01000	mov 	r1, 0x0
    5c:	e0040690	mul 	r4, r0, r6
    60:	e0844001	add 	r4,	r4,	r1
    64:	e59f4084	ldr 	r4, [pc, 0x84] <- 0xaaaa
    68:	e1c340b0	strh	r4, [r3, 0x0]
    6c:	e2833002	add 	r3,	r3,	0x2
    70:	e2811001	add 	r1,	r1,	0x1
    74:	e1510006	cmp 	r1,	r6
    78:	bafffff7	blt	0x5c
    7c:	e2800001	add 	r0,	r0,	0x1
    80:	e1500007	cmp 	r0,	r7
    84:	bafffff3	blt	0x58
    88:	e1a0e00f	mov 	lr, pc
    8c:	e59ff030	ldr 	pc, [pc, 0x30] <- 0x200330fc
    90:	e1a09000	mov 	r9, r0
    94:	e3a00001	mov 	r0, 0x1
    98:	e1a0e00f	mov 	lr, pc
    9c:	e59ff024	ldr 	pc, [pc, 0x24] <- 0x200a06c0
    a0:	e3590000	cmp 	r9,	0x0
    a4:	1a000003	bne	0xb8
    a8:	e3a00000	mov 	r0, 0x0
    ac:	e1a0e00f	mov 	lr, pc
    b0:	e59ff018	ldr 	pc, [pc, 0x18] <- 0x2004f638
    b4:	eaffffd7	b	0x18
    b8:	e28dd020	add 	sp,	sp,	0x20
    bc:	e8bd8fff	ldmia	sp!, {r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, sl, fp, pc}
    c0:	200fea64
    c4:	200330fc
    c8:	200a06c0
    cc:	20054684	andcs 	r4,	r5,	r4 lsl #13
    d0:	2004f638
    d4:	2004fad0
    d8:	0000f81f
    dc:	0000fe5b
    e0:	6c6c6548	copvs
    e4:	0000216f
    e8:	74736554	ldrvcb	error 'write back' set
    ec:	00000000
    f0:	0000aaaa
    f4:	00000000
    f8:	0000ffff
