     0:	ea00000b	b	0x34
     4:	e92d4006	stmdb	sp!, {r1, r2, lr}
     8:	e59f0018	ldr 	r0, [pc, 0x18] <- 0x0
     c:	e59f1018	ldr 	r1, [pc, 0x18] <- 0x10dcd
    10:	e0020190	mul 	r2, r0, r1
    14:	e59f1014	ldr 	r1, [pc, 0x14] <- 0x5
    18:	e0820001	add 	r0,	r2,	r1
    1c:	e58f0004	str 	r0, [pc, 0x4] <- 0x0
    20:	e1a00820	mov 	r0, r0 lsr #16
    24:	e8bd8006	ldmia	sp!, {r1, r2, pc}
    28:	00000000
    2c:	00010dcd
    30:	00000005
    34:	e92d4fff	stmdb	sp!, {r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, sl, fp, lr}
    38:	e24dd020	sub 	sp,	sp,	0x20
    3c:	e59f408c	ldr 	r4, [pc, 0x8c] <- 0x200fea64
    40:	e5945000	ldr 	r5, [r4]
    44:	e8b500c0	ldmia	r5!, {r6, r7}
    48:	e1a03005	mov 	r3, r5
    4c:	e3a00000	mov 	r0, 0x0
    50:	e3a01000	mov 	r1, 0x0
    54:	e1a09000	mov 	r9, r0
    58:	ebffffe9	bl	0x4
    5c:	e1a04000	mov 	r4, r0
    60:	e1a00009	mov 	r0, r9
    64:	e1c340b0	strh	r4, [r3, 0x0]
    68:	e2833002	add 	r3,	r3,	0x2
    6c:	e2811001	add 	r1,	r1,	0x1
    70:	e1510006	cmp 	r1,	r6
    74:	bafffff6	blt	0x54
    78:	e2800001	add 	r0,	r0,	0x1
    7c:	e1500007	cmp 	r0,	r7
    80:	bafffff2	blt	0x50
    84:	e3a00000	mov 	r0, 0x0
    88:	e1a0e00f	mov 	lr, pc
    8c:	e59ff038	ldr 	pc, [pc, 0x38] <- 0x2004f638
    90:	e92d00ff	stmdb	sp!, {r0, r1, r2, r3, r4, r5, r6, r7}
    94:	e1a0e00f	mov 	lr, pc
    98:	e59ff028	ldr 	pc, [pc, 0x28] <- 0x200330fc
    9c:	e1a09000	mov 	r9, r0
    a0:	e3a00001	mov 	r0, 0x1
    a4:	e1a0e00f	mov 	lr, pc
    a8:	e59ff014	ldr 	pc, [pc, 0x14] <- 0x200a06c0
    ac:	e8bd00ff	ldmia	sp!, {r0, r1, r2, r3, r4, r5, r6, r7}
    b0:	e3590000	cmp 	r9,	0x0
    b4:	1a000000	bne	0xbc
    b8:	eaffffdf	b	0x3c
    bc:	e28dd020	add 	sp,	sp,	0x20
    c0:	e8bd8fff	ldmia	sp!, {r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, sl, fp, pc}
    c4:	200a06c0
    c8:	200330fc
    cc:	2004f638
    d0:	200fea64
    d4:	80000f54	andhi 	r0,	r0,	r4 asr pc
    d8:	0000aaaa
    dc:	00000000
    e0:	0000ffff
