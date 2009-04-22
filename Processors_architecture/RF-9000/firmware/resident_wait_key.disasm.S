     0:	e92d4fff	stmdb	sp!, {r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, sl, fp, lr}
     4:	e24dd020	sub 	sp,	sp,	0x20
     8:	e1a0e00f	mov 	lr, pc
     c:	e59ff024	ldr 	pc, [pc, 0x24] <- 0x200330fc
    10:	e1a09000	mov 	r9, r0
    14:	e3a00001	mov 	r0, 0x1
    18:	e1a0e00f	mov 	lr, pc
    1c:	e59ff010	ldr 	pc, [pc, 0x10] <- 0x200a06c0
    20:	e3590000	cmp 	r9,	0x0
    24:	1a000000	bne	0x2c
    28:	eafffff6	b	0x8
    2c:	e28dd020	add 	sp,	sp,	0x20
    30:	e8bd8fff	ldmia	sp!, {r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, sl, fp, pc}
    34:	200a06c0
    38:	200330fc
