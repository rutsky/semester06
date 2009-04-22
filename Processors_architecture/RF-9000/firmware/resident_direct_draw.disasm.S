     0:	e92d4fff	stmdb	sp!, {r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, sl, fp, lr}
     4:	e24dd020	sub 	sp,	sp,	0x20
     8:	e59f4068	ldr 	r4, [pc, 0x68] <- 0x80000f54
     c:	e5945000	ldr 	r5, [r4]
    10:	e59f006c	ldr 	r0, [pc, 0x6c] <- 0xffff
    14:	e59f1064	ldr 	r1, [pc, 0x64] <- 0x0
    18:	e1c500b0	strh	r0, [r5, 0x0]
    1c:	e1c510b2	strh	r1, [r5, 0x2]
    20:	e1c500b4	strh	r0, [r5, 0x4]
    24:	e1c510b6	strh	r1, [r5, 0x6]
    28:	e1c500b8	strh	r0, [r5, 0x8]
    2c:	e1c510ba	strh	r1, [r5, 0xa]
    30:	e1c500bc	strh	r0, [r5, 0xc]
    34:	e1c510be	strh	r1, [r5, 0xe]
    38:	e92d007f	stmdb	sp!, {r0, r1, r2, r3, r4, r5, r6}
    3c:	e1a0e00f	mov 	lr, pc
    40:	e59ff028	ldr 	pc, [pc, 0x28] <- 0x200330fc
    44:	e1a09000	mov 	r9, r0
    48:	e3a00001	mov 	r0, 0x1
    4c:	e1a0e00f	mov 	lr, pc
    50:	e59ff014	ldr 	pc, [pc, 0x14] <- 0x200a06c0
    54:	e8bd007f	ldmia	sp!, {r0, r1, r2, r3, r4, r5, r6}
    58:	e3590000	cmp 	r9,	0x0
    5c:	1a000000	bne	0x64
    60:	eaffffe8	b	0x8
    64:	e28dd020	add 	sp,	sp,	0x20
    68:	e8bd8fff	ldmia	sp!, {r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, sl, fp, pc}
    6c:	200a06c0
    70:	200330fc
    74:	200fea64	andcs 	lr,	pc,	r4 ror #20
    78:	80000f54
    7c:	0000aaaa
    80:	00000000
    84:	0000ffff
