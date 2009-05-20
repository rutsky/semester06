     0:	e92d4fff	stmdb	sp!, {r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, sl, fp, lr}
     4:	e24dd020	sub 	sp,	sp,	0x20
     8:	e59f40b0	ldr 	r4, [pc, 0xb0] <- 0x200fea64
     c:	e5945000	ldr 	r5, [r4]
    10:	e8b500c0	ldmia	r5!, {r6, r7}
    14:	e59f00b4	ldr 	r0, [pc, 0xb4] <- 0xffff
    18:	e59f10ac	ldr 	r1, [pc, 0xac] <- 0x0
    1c:	e1c500b0	strh	r0, [r5, 0x0]
    20:	e1c510b2	strh	r1, [r5, 0x2]
    24:	e1c500b4	strh	r0, [r5, 0x4]
    28:	e1c510b6	strh	r1, [r5, 0x6]
    2c:	e1c500b8	strh	r0, [r5, 0x8]
    30:	e1c510ba	strh	r1, [r5, 0xa]
    34:	e1c500bc	strh	r0, [r5, 0xc]
    38:	e1c510be	strh	r1, [r5, 0xe]
    3c:	e1a03005	mov 	r3, r5
    40:	e3a00000	mov 	r0, 0x0
    44:	e3a01000	mov 	r1, 0x0
    48:	e0040690	mul 	r4, r0, r6
    4c:	e0844001	add 	r4,	r4,	r1
    50:	e59f4070	ldr 	r4, [pc, 0x70] <- 0xaaaa
    54:	e1c340b0	strh	r4, [r3, 0x0]
    58:	e2833002	add 	r3,	r3,	0x2
    5c:	e2811001	add 	r1,	r1,	0x1
    60:	e1510006	cmp 	r1,	r6
    64:	bafffff7	blt	0x48
    68:	e2800001	add 	r0,	r0,	0x1
    6c:	e1500007	cmp 	r0,	r7
    70:	bafffff3	blt	0x44
    74:	e3a00000	mov 	r0, 0x0
    78:	e1a0e00f	mov 	lr, pc
    7c:	e59ff038	ldr 	pc, [pc, 0x38] <- 0x2004f638
    80:	e92d007f	stmdb	sp!, {r0, r1, r2, r3, r4, r5, r6}
    84:	e1a0e00f	mov 	lr, pc
    88:	e59ff028	ldr 	pc, [pc, 0x28] <- 0x200330fc
    8c:	e1a09000	mov 	r9, r0
    90:	e3a00001	mov 	r0, 0x1
    94:	e1a0e00f	mov 	lr, pc
    98:	e59ff014	ldr 	pc, [pc, 0x14] <- 0x200a06c0
    9c:	e8bd007f	ldmia	sp!, {r0, r1, r2, r3, r4, r5, r6}
    a0:	e3590000	cmp 	r9,	0x0
    a4:	1a000000	bne	0xac
    a8:	eaffffd6	b	0x8
    ac:	e28dd020	add 	sp,	sp,	0x20
    b0:	e8bd8fff	ldmia	sp!, {r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, sl, fp, pc}
    b4:	200a06c0
    b8:	200330fc
    bc:	2004f638
    c0:	200fea64
    c4:	80000f54	andhi 	r0,	r0,	r4 asr pc
    c8:	0000aaaa
    cc:	00000000
    d0:	0000ffff
