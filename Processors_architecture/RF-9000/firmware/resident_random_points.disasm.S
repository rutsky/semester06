     0:	e92d4fff	stmdb	sp!, {r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, sl, fp, lr}
     4:	e24dd020	sub 	sp,	sp,	0x20
     8:	e59f40a8	ldr 	r4, [pc, 0xa8] <- 0x200fea64
     c:	e5945000	ldr 	r5, [r4]
    10:	e8b500c0	ldmia	r5!, {r6, r7}
    14:	e1a03005	mov 	r3, r5
    18:	e3a00000	mov 	r0, 0x0
    1c:	e3a01000	mov 	r1, 0x0
    20:	e59f90a4	ldr 	r9, [pc, 0xa4] <- 0x20010001
    24:	e5998000	ldr 	r8, [r9]
    28:	e5984000	ldr 	r4, [r8]
    2c:	e1c340b0	strh	r4, [r3, 0x0]
    30:	e2888002	add 	r8,	r8,	0x2
    34:	e59f4098	ldr 	r4, [pc, 0x98] <- 0x200b0000
    38:	e5944000	ldr 	r4, [r4]
    3c:	e1580004	cmp 	r8,	r4
    40:	ba000001	blt	0x4c
    44:	e59f4084	ldr 	r4, [pc, 0x84] <- 0x20010001
    48:	e5948000	ldr 	r8, [r4]
    4c:	e5898000	str 	r8, [r9]
    50:	e2833002	add 	r3,	r3,	0x2
    54:	e2811001	add 	r1,	r1,	0x1
    58:	e1510006	cmp 	r1,	r6
    5c:	baffffef	blt	0x20
    60:	e2800001	add 	r0,	r0,	0x1
    64:	e1500007	cmp 	r0,	r7
    68:	baffffeb	blt	0x1c
    6c:	e3a00000	mov 	r0, 0x0
    70:	e1a0e00f	mov 	lr, pc
    74:	e59ff038	ldr 	pc, [pc, 0x38] <- 0x2004f638
    78:	e92d007f	stmdb	sp!, {r0, r1, r2, r3, r4, r5, r6}
    7c:	e1a0e00f	mov 	lr, pc
    80:	e59ff028	ldr 	pc, [pc, 0x28] <- 0x200330fc
    84:	e1a09000	mov 	r9, r0
    88:	e3a00001	mov 	r0, 0x1
    8c:	e1a0e00f	mov 	lr, pc
    90:	e59ff014	ldr 	pc, [pc, 0x14] <- 0x200a06c0
    94:	e8bd007f	ldmia	sp!, {r0, r1, r2, r3, r4, r5, r6}
    98:	e3590000	cmp 	r9,	0x0
    9c:	1a000000	bne	0xa4
    a0:	eaffffd8	b	0x8
    a4:	e28dd020	add 	sp,	sp,	0x20
    a8:	e8bd8fff	ldmia	sp!, {r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, sl, fp, pc}
    ac:	200a06c0
    b0:	200330fc
    b4:	2004f638
    b8:	200fea64
    bc:	80000f54	andhi 	r0,	r0,	r4 asr pc
    c0:	0000aaaa
    c4:	00000000
    c8:	0000ffff
    cc:	20010001
    d0:	20010001
    d4:	200b0000
