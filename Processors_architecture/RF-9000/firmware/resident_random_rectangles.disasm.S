     0:	ea000017	b	0x64
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
    34:	e92d4004	stmdb	sp!, {r2, lr}
    38:	e3a02000	mov 	r2, 0x0
    3c:	e1500001	cmp 	r0,	r1
    40:	ba000004	blt	0x58
    44:	e3500000	cmp 	r0,	0x0
    48:	0a000002	beq	0x58
    4c:	e2822001	add 	r2,	r2,	0x1
    50:	e0400001	sub 	r0,	r0,	r1
    54:	eafffff8	b	0x3c
    58:	e1a01000	mov 	r1, r0
    5c:	e1a00002	mov 	r0, r2
    60:	e8bd8004	ldmia	sp!, {r2, pc}
    64:	e92d4fff	stmdb	sp!, {r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, sl, fp, lr}
    68:	e24dd020	sub 	sp,	sp,	0x20
    6c:	e59f40f0	ldr 	r4, [pc, 0xf0] <- 0x200fea64
    70:	e594a000	ldr 	sl, [r4]
    74:	e8ba0300	ldmia	sl!, {r8, r9}
    78:	ebffffe1	bl	0x4
    7c:	e1a01008	mov 	r1, r8
    80:	ebffffeb	bl	0x34
    84:	e1a04001	mov 	r4, r1
    88:	ebffffdd	bl	0x4
    8c:	e1a01008	mov 	r1, r8
    90:	ebffffe7	bl	0x34
    94:	e1a06001	mov 	r6, r1
    98:	ebffffd9	bl	0x4
    9c:	e1a01009	mov 	r1, r9
    a0:	ebffffe3	bl	0x34
    a4:	e1a05001	mov 	r5, r1
    a8:	ebffffd5	bl	0x4
    ac:	e1a01009	mov 	r1, r9
    b0:	ebffffdf	bl	0x34
    b4:	e1a07001	mov 	r7, r1
    b8:	e1540006	cmp 	r4,	r6
    bc:	ba000002	blt	0xcc
    c0:	e1a00004	mov 	r0, r4
    c4:	e1a04006	mov 	r4, r6
    c8:	e1a06000	mov 	r6, r0
    cc:	e1550007	cmp 	r5,	r7
    d0:	ba000002	blt	0xe0
    d4:	e1a00005	mov 	r0, r5
    d8:	e1a05007	mov 	r5, r7
    dc:	e1a07000	mov 	r7, r0
    e0:	ebffffc7	bl	0x4
    e4:	e1a0b000	mov 	fp, r0
    e8:	e1a00005	mov 	r0, r5
    ec:	e1a01004	mov 	r1, r4
    f0:	e0020890	mul 	r2, r0, r8
    f4:	e0822001	add 	r2,	r2,	r1
    f8:	e08a3082	add 	r3,	sl,	r2 lsl #1
    fc:	e1c3b0b0	strh	fp, [r3, 0x0]
   100:	e2811001	add 	r1,	r1,	0x1
   104:	e1510006	cmp 	r1,	r6
   108:	bafffff8	blt	0xf0
   10c:	e2800001	add 	r0,	r0,	0x1
   110:	e1500007	cmp 	r0,	r7
   114:	bafffff4	blt	0xec
   118:	e3a00000	mov 	r0, 0x0
   11c:	e1a0e00f	mov 	lr, pc
   120:	e59ff038	ldr 	pc, [pc, 0x38] <- 0x2004f638
   124:	e92d01ff	stmdb	sp!, {r0, r1, r2, r3, r4, r5, r6, r7, r8}
   128:	e1a0e00f	mov 	lr, pc
   12c:	e59ff028	ldr 	pc, [pc, 0x28] <- 0x200330fc
   130:	e1a09000	mov 	r9, r0
   134:	e3a00001	mov 	r0, 0x1
   138:	e1a0e00f	mov 	lr, pc
   13c:	e59ff014	ldr 	pc, [pc, 0x14] <- 0x200a06c0
   140:	e8bd01ff	ldmia	sp!, {r0, r1, r2, r3, r4, r5, r6, r7, r8}
   144:	e3590000	cmp 	r9,	0x0
   148:	1a000000	bne	0x150
   14c:	eaffffc6	b	0x6c
   150:	e28dd020	add 	sp,	sp,	0x20
   154:	e8bd8fff	ldmia	sp!, {r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, sl, fp, pc}
   158:	200a06c0
   15c:	200330fc
   160:	2004f638
   164:	200fea64
   168:	80000f54	andhi 	r0,	r0,	r4 asr pc
   16c:	0000aaaa
   170:	00000000
   174:	0000ffff
