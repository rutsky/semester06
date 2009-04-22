     0:	e92d4fff	stmdb	sp!, {r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, sl, fp, lr}
     4:	e24dd020	sub 	sp,	sp,	0x20
     8:	e59f505f	ldr 	r5, [pc, 0x5f] <- 0xf5420
     c:	e59f0068	ldr 	r0, [pc, 0x68] <- 0xffff
    10:	e59f1060	ldr 	r1, [pc, 0x60] <- 0x0
    14:	e1c500b0	strh	r0, [r5, 0x0]
    18:	e1c510b2	strh	r1, [r5, 0x2]
    1c:	e1c500b4	strh	r0, [r5, 0x4]
    20:	e1c510b6	strh	r1, [r5, 0x6]
    24:	e1c500b8	strh	r0, [r5, 0x8]
    28:	e1c510ba	strh	r1, [r5, 0xa]
    2c:	e1c500bc	strh	r0, [r5, 0xc]
    30:	e1c510be	strh	r1, [r5, 0xe]
    34:	e92d007f	stmdb	sp!, {r0, r1, r2, r3, r4, r5, r6}
    38:	e1a0e00f	mov 	lr, pc
    3c:	e59ff028	ldr 	pc, [pc, 0x28] <- 0x200330fc
    40:	e1a09000	mov 	r9, r0
    44:	e3a00001	mov 	r0, 0x1
    48:	e1a0e00f	mov 	lr, pc
    4c:	e59ff014	ldr 	pc, [pc, 0x14] <- 0x200a06c0
    50:	e8bd007f	ldmia	sp!, {r0, r1, r2, r3, r4, r5, r6}
    54:	e3590000	cmp 	r9,	0x0
    58:	1a000000	bne	0x60
    5c:	eaffffe9	b	0x8
    60:	e28dd020	add 	sp,	sp,	0x20
    64:	e8bd8fff	ldmia	sp!, {r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, sl, fp, pc}
    68:	200a06c0
    6c:	200330fc
    70:	80000f54	andhi 	r0,	r0,	r4 asr pc
    74:	0000aaaa
    78:	00000000
    7c:	0000ffff
