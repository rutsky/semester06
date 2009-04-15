     0:	e92d4fff	stmdb	sp!, {r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, sl, fp, lr}
     4:	e24dd020	sub 	sp,	sp,	0x20
     8:	e3a00050	mov 	r0, 0x50
     c:	e3a01058	mov 	r1, 0x58
    10:	e1a0e00f	mov 	lr, pc
    14:	e59ff030	ldr 	pc, [pc, 0x30] <- 0x200573c8
    18:	e1a0e00f	mov 	lr, pc
    1c:	e59ff020	ldr 	pc, [pc, 0x20] <- 0x200330fc
    20:	e1a09000	mov 	r9, r0
    24:	e3a00001	mov 	r0, 0x1
    28:	e1a0e00f	mov 	lr, pc
    2c:	e59ff014	ldr 	pc, [pc, 0x14] <- 0x200a06c0
    30:	e3590000	cmp 	r9,	0x0
    34:	1a000000	bne	0x3c
    38:	eafffff6	b	0x18
    3c:	e28dd020	add 	sp,	sp,	0x20
    40:	e8bd8fff	ldmia	sp!, {r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, sl, fp, pc}
    44:	200330fc
    48:	200a06c0
    4c:	200573c8
    50:	73726946	cmnvc 	r2,	0x118000
    54:	00000074
    58:	6f636553	swivs
    5c:	0000646e
