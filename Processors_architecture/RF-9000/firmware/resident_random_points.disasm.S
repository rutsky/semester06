     0:	ea00000a	b	0x30
     4:	e92d4006	stmdb	sp!, {r1, r2, lr}
     8:	e59f0014	ldr 	r0, [pc, 0x14] <- 0x0
     c:	e59f1014	ldr 	r1, [pc, 0x14] <- 0x10dcd
    10:	e0020190	mul 	r2, r0, r1
    14:	e59f1010	ldr 	r1, [pc, 0x10] <- 0x5
    18:	e0820001	add 	r0,	r2,	r1
    1c:	e58f0000	str 	r0, [pc]
    20:	e8bd8006	ldmia	sp!, {r1, r2, pc}
    24:	00000000
    28:	00010dcd
    2c:	00000005
    30:	e92d4fff	stmdb	sp!, {r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, sl, fp, lr}
    34:	e24dd020	sub 	sp,	sp,	0x20
    38:	e59f408c	ldr 	r4, [pc, 0x8c] <- 0x200fea64
    3c:	e5945000	ldr 	r5, [r4]
    40:	e8b500c0	ldmia	r5!, {r6, r7}
    44:	e1a03005	mov 	r3, r5
    48:	e3a00000	mov 	r0, 0x0
    4c:	e3a01000	mov 	r1, 0x0
    50:	e1a09000	mov 	r9, r0
    54:	ebffffea	bl	0x4
    58:	e1a04820	mov 	r4, r0 lsr #16
    5c:	e1a00009	mov 	r0, r9
    60:	e1c340b0	strh	r4, [r3, 0x0]
    64:	e2833002	add 	r3,	r3,	0x2
    68:	e2811001	add 	r1,	r1,	0x1
    6c:	e1510006	cmp 	r1,	r6
    70:	bafffff6	blt	0x50
    74:	e2800001	add 	r0,	r0,	0x1
    78:	e1500007	cmp 	r0,	r7
    7c:	bafffff2	blt	0x4c
    80:	e3a00000	mov 	r0, 0x0
    84:	e1a0e00f	mov 	lr, pc
    88:	e59ff038	ldr 	pc, [pc, 0x38] <- 0x2004f638
    8c:	e92d00ff	stmdb	sp!, {r0, r1, r2, r3, r4, r5, r6, r7}
    90:	e1a0e00f	mov 	lr, pc
    94:	e59ff028	ldr 	pc, [pc, 0x28] <- 0x200330fc
    98:	e1a09000	mov 	r9, r0
    9c:	e3a00001	mov 	r0, 0x1
    a0:	e1a0e00f	mov 	lr, pc
    a4:	e59ff014	ldr 	pc, [pc, 0x14] <- 0x200a06c0
    a8:	e8bd00ff	ldmia	sp!, {r0, r1, r2, r3, r4, r5, r6, r7}
    ac:	e3590000	cmp 	r9,	0x0
    b0:	1a000000	bne	0xb8
    b4:	eaffffdf	b	0x38
    b8:	e28dd020	add 	sp,	sp,	0x20
    bc:	e8bd8fff	ldmia	sp!, {r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, sl, fp, pc}
    c0:	200a06c0
    c4:	200330fc
    c8:	2004f638
    cc:	200fea64
    d0:	80000f54	andhi 	r0,	r0,	r4 asr pc
    d4:	0000aaaa
    d8:	00000000
    dc:	0000ffff
