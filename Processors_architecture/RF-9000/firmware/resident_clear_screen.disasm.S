     0:	e92d4fff	stmdb	sp!, {r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, sl, fp, lr}
     4:	e24dd000	sub 	sp,	sp,	0x0
     8:	e3a00000	mov 	r0, 0x0
     c:	e59f1014	ldr 	r1, [pc, 0x14] <- 0x0
    10:	e1a0e00f	mov 	lr, pc
    14:	e59ff008	ldr 	pc, [pc, 0x8] <- 0x2004fad0
    18:	e28dd000	add 	sp,	sp,	0x0
    1c:	e8bd8fff	ldmia	sp!, {r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, sl, fp, pc}
    20:	200fea64	andcs 	lr,	pc,	r4 ror #20
    24:	2004fad0
    28:	00000000
