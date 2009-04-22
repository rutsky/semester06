     0:	e92d4fff	stmdb	sp!, {r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, sl, fp, lr}
     4:	e24dd020	sub 	sp,	sp,	0x20
     8:	e59f5067	ldr 	r5, [pc, 0x67] <- 0xfea6420
     c:	e59f6067	ldr 	r6, [pc, 0x67] <- 0xf5420
    10:	e5865000	str 	r5, [r6]
    14:	e59f006c	ldr 	r0, [pc, 0x6c] <- 0xffff
    18:	e59f1064	ldr 	r1, [pc, 0x64] <- 0x0
    1c:	e1c500b0	strh	r0, [r5, 0x0]
    20:	e1c510b2	strh	r1, [r5, 0x2]
    24:	e1c500b4	strh	r0, [r5, 0x4]
    28:	e1c510b6	strh	r1, [r5, 0x6]
    2c:	e1c500b8	strh	r0, [r5, 0x8]
    30:	e1c510ba	strh	r1, [r5, 0xa]
    34:	e1c500bc	strh	r0, [r5, 0xc]
    38:	e1c510be	strh	r1, [r5, 0xe]
    3c:	e92d007f	stmdb	sp!, {r0, r1, r2, r3, r4, r5, r6}
    40:	e1a0e00f	mov 	lr, pc
    44:	e59ff028	ldr 	pc, [pc, 0x28] <- 0x200330fc
    48:	e1a09000	mov 	r9, r0
    4c:	e3a00001	mov 	r0, 0x1
    50:	e1a0e00f	mov 	lr, pc
    54:	e59ff014	ldr 	pc, [pc, 0x14] <- 0x200a06c0
    58:	e8bd007f	ldmia	sp!, {r0, r1, r2, r3, r4, r5, r6}
    5c:	e3590000	cmp 	r9,	0x0
    60:	1a000000	bne	0x68
    64:	eaffffe7	b	0x8
    68:	e28dd020	add 	sp,	sp,	0x20
    6c:	e8bd8fff	ldmia	sp!, {r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, sl, fp, pc}
    70:	200a06c0
    74:	200330fc
    78:	200fea64
    7c:	80000f54	andhi 	r0,	r0,	r4 asr pc
    80:	0000aaaa
    84:	00000000
    88:	0000ffff
