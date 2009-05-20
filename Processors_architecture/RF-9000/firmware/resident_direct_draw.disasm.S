     0:	e92d4fff	stmdb	sp!, {r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, sl, fp, lr}
     4:	e24dd020	sub 	sp,	sp,	0x20
     8:	e59f4078	ldr 	r4, [pc, 0x78] <- 0x200fea64
     c:	e5945000	ldr 	r5, [r4]
    10:	e8b50300	ldmia	r5!, {r8, r9}
    14:	e59f007c	ldr 	r0, [pc, 0x7c] <- 0xffff
    18:	e59f1074	ldr 	r1, [pc, 0x74] <- 0x0
    1c:	e1c500b0	strh	r0, [r5, 0x0]
    20:	e1c510b2	strh	r1, [r5, 0x2]
    24:	e1c500b4	strh	r0, [r5, 0x4]
    28:	e1c510b6	strh	r1, [r5, 0x6]
    2c:	e1c500b8	strh	r0, [r5, 0x8]
    30:	e1c510ba	strh	r1, [r5, 0xa]
    34:	e1c500bc	strh	r0, [r5, 0xc]
    38:	e1c510be	strh	r1, [r5, 0xe]
    3c:	e3a00000	mov 	r0, 0x0
    40:	e1a0e00f	mov 	lr, pc
    44:	e59ff038	ldr 	pc, [pc, 0x38] <- 0x2004f638
    48:	e92d007f	stmdb	sp!, {r0, r1, r2, r3, r4, r5, r6}
    4c:	e1a0e00f	mov 	lr, pc
    50:	e59ff028	ldr 	pc, [pc, 0x28] <- 0x200330fc
    54:	e1a09000	mov 	r9, r0
    58:	e3a00001	mov 	r0, 0x1
    5c:	e1a0e00f	mov 	lr, pc
    60:	e59ff014	ldr 	pc, [pc, 0x14] <- 0x200a06c0
    64:	e8bd007f	ldmia	sp!, {r0, r1, r2, r3, r4, r5, r6}
    68:	e3590000	cmp 	r9,	0x0
    6c:	1a000000	bne	0x74
    70:	eaffffe4	b	0x8
    74:	e28dd020	add 	sp,	sp,	0x20
    78:	e8bd8fff	ldmia	sp!, {r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, sl, fp, pc}
    7c:	200a06c0
    80:	200330fc
    84:	2004f638
    88:	200fea64
    8c:	80000f54	andhi 	r0,	r0,	r4 asr pc
    90:	0000aaaa
    94:	00000000
    98:	0000ffff
