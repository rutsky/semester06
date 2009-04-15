     0:	e92d4fff	stmdb	sp!, {r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, sl, fp, lr}
     4:	e24dd020	sub 	sp,	sp,	0x20
     8:	e59f50a0	ldr 	r5, [pc, 0xa0] <- 0x200fea64
     c:	e3a06064	mov 	r6, 0x64
    10:	e3a07064	mov 	r7, 0x64
    14:	e2855008	add 	r5,	r5,	0x8
    18:	e59f00c4	ldr 	r0, [pc, 0xc4] <- 0xffff
    1c:	e59f10bc	ldr 	r1, [pc, 0xbc] <- 0x0
    20:	e1c500b0	strh	r0, [r5, 0x0]
    24:	e1c510b2	strh	r1, [r5, 0x2]
    28:	e1c500b4	strh	r0, [r5, 0x4]
    2c:	e1c510b6	strh	r1, [r5, 0x6]
    30:	e1c500b8	strh	r0, [r5, 0x8]
    34:	e1c510ba	strh	r1, [r5, 0xa]
    38:	e1c500bc	strh	r0, [r5, 0xc]
    3c:	e1c510be	strh	r1, [r5, 0xe]
    40:	e1a03005	mov 	r3, r5
    44:	e3a00000	mov 	r0, 0x0
    48:	e3a01000	mov 	r1, 0x0
    4c:	e0040690	mul 	r4, r0, r6
    50:	e0844001	add 	r4,	r4,	r1
    54:	e59f4080	ldr 	r4, [pc, 0x80] <- 0xaaaa
    58:	e1c340b0	strh	r4, [r3, 0x0]
    5c:	e2833002	add 	r3,	r3,	0x2
    60:	e2811001	add 	r1,	r1,	0x1
    64:	e1510006	cmp 	r1,	r6
    68:	bafffff7	blt	0x4c
    6c:	e2800001	add 	r0,	r0,	0x1
    70:	e1500007	cmp 	r0,	r7
    74:	bafffff3	blt	0x48
    78:	e1a0e00f	mov 	lr, pc
    7c:	e59ff030	ldr 	pc, [pc, 0x30] <- 0x200330fc
    80:	e1a09000	mov 	r9, r0
    84:	e3a00001	mov 	r0, 0x1
    88:	e1a0e00f	mov 	lr, pc
    8c:	e59ff024	ldr 	pc, [pc, 0x24] <- 0x200a06c0
    90:	e3590000	cmp 	r9,	0x0
    94:	1a000003	bne	0xa8
    98:	e3a00000	mov 	r0, 0x0
    9c:	e1a0e00f	mov 	lr, pc
    a0:	e59ff018	ldr 	pc, [pc, 0x18] <- 0x2004f638
    a4:	eaffffd7	b	0x8
    a8:	e28dd020	add 	sp,	sp,	0x20
    ac:	e8bd8fff	ldmia	sp!, {r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, sl, fp, pc}
    b0:	200fea64
    b4:	200330fc
    b8:	200a06c0
    bc:	20054684	andcs 	r4,	r5,	r4 lsl #13
    c0:	2004f638
    c4:	0000f81f
    c8:	0000fe5b
    cc:	6c6c6548	copvs
    d0:	0000216f
    d4:	74736554	ldrvcb	error 'write back' set
    d8:	00000000
    dc:	0000aaaa
    e0:	00000000
    e4:	0000ffff
