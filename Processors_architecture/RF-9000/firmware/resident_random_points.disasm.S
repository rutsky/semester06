     0:	e92d4fff	stmdb	sp!, {r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, sl, fp, lr}
     4:	e24dd020	sub 	sp,	sp,	0x20
     8:	e59f4088	ldr 	r4, [pc, 0x88] <- 0x200fea64
     c:	e5945000	ldr 	r5, [r4]
    10:	e8b500c0	ldmia	r5!, {r6, r7}
    14:	e59f4090	ldr 	r4, [pc, 0x90] <- 0x0
    18:	e1a03005	mov 	r3, r5
    1c:	e3a00000	mov 	r0, 0x0
    20:	e3a01000	mov 	r1, 0x0
    24:	e1c340b0	strh	r4, [r3, 0x0]
    28:	e2844025	add 	r4,	r4,	0x25
    2c:	e2833002	add 	r3,	r3,	0x2
    30:	e2811001	add 	r1,	r1,	0x1
    34:	e1510006	cmp 	r1,	r6
    38:	bafffff9	blt	0x24
    3c:	e2800001	add 	r0,	r0,	0x1
    40:	e1500007	cmp 	r0,	r7
    44:	bafffff5	blt	0x20
    48:	e58f405c	str 	r4, [pc, 0x5c] <- 0x0
    4c:	e3a00000	mov 	r0, 0x0
    50:	e1a0e00f	mov 	lr, pc
    54:	e59ff038	ldr 	pc, [pc, 0x38] <- 0x2004f638
    58:	e92d007f	stmdb	sp!, {r0, r1, r2, r3, r4, r5, r6}
    5c:	e1a0e00f	mov 	lr, pc
    60:	e59ff028	ldr 	pc, [pc, 0x28] <- 0x200330fc
    64:	e1a09000	mov 	r9, r0
    68:	e3a00001	mov 	r0, 0x1
    6c:	e1a0e00f	mov 	lr, pc
    70:	e59ff014	ldr 	pc, [pc, 0x14] <- 0x200a06c0
    74:	e8bd007f	ldmia	sp!, {r0, r1, r2, r3, r4, r5, r6}
    78:	e3590000	cmp 	r9,	0x0
    7c:	1a000000	bne	0x84
    80:	eaffffe0	b	0x8
    84:	e28dd020	add 	sp,	sp,	0x20
    88:	e8bd8fff	ldmia	sp!, {r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, sl, fp, pc}
    8c:	200a06c0
    90:	200330fc
    94:	2004f638
    98:	200fea64
    9c:	80000f54	andhi 	r0,	r0,	r4 asr pc
    a0:	0000aaaa
    a4:	00000000
    a8:	0000ffff
    ac:	00000000
