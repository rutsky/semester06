     0:	e92d4fff	stmdb	sp!, {r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, sl, fp, lr}
     4:	e24dd020	sub 	sp,	sp,	0x20
     8:	e59f00f8	ldr 	r0, [pc, 0xf8] <- 0xaaaa
     c:	e1cd00b0	strh	r0, [sp, 0x0]
    10:	e59f00f4	ldr 	r0, [pc, 0xf4] <- 0x0
    14:	e59d1000	ldr 	r1, [sp]
    18:	e2801001	add 	r1,	r0,	0x1
    1c:	e0011000	and 	r1,	r1,	r0
    20:	e58d1000	str 	r1, [sp]
    24:	e3a00000	mov 	r0, 0x0
    28:	e1a0e00f	mov 	lr, pc
    2c:	e59ff0b8	ldr 	pc, [pc, 0xb8] <- 0x2004fad0
    30:	e59f50a0	ldr 	r5, [pc, 0xa0] <- 0x200fea64
    34:	e3a06064	mov 	r6, 0x64
    38:	e3a07064	mov 	r7, 0x64
    3c:	e2855008	add 	r5,	r5,	0x8
    40:	e59f00c8	ldr 	r0, [pc, 0xc8] <- 0xffff
    44:	e59f10c0	ldr 	r1, [pc, 0xc0] <- 0x0
    48:	e1c500b0	strh	r0, [r5, 0x0]
    4c:	e1c510b2	strh	r1, [r5, 0x2]
    50:	e1c500b4	strh	r0, [r5, 0x4]
    54:	e1c510b6	strh	r1, [r5, 0x6]
    58:	e1c500b8	strh	r0, [r5, 0x8]
    5c:	e1c510ba	strh	r1, [r5, 0xa]
    60:	e1c500bc	strh	r0, [r5, 0xc]
    64:	e1c510be	strh	r1, [r5, 0xe]
    68:	e1a03005	mov 	r3, r5
    6c:	e3a00000	mov 	r0, 0x0
    70:	e3a01000	mov 	r1, 0x0
    74:	e0040690	mul 	r4, r0, r6
    78:	e0844001	add 	r4,	r4,	r1
    7c:	e59f4084	ldr 	r4, [pc, 0x84] <- 0xaaaa
    80:	e1c340b0	strh	r4, [r3, 0x0]
    84:	e2833002	add 	r3,	r3,	0x2
    88:	e2811001	add 	r1,	r1,	0x1
    8c:	e1510006	cmp 	r1,	r6
    90:	bafffff7	blt	0x74
    94:	e2800001	add 	r0,	r0,	0x1
    98:	e1500007	cmp 	r0,	r7
    9c:	bafffff3	blt	0x70
    a0:	e1a0e00f	mov 	lr, pc
    a4:	e59ff030	ldr 	pc, [pc, 0x30] <- 0x200330fc
    a8:	e1a09000	mov 	r9, r0
    ac:	e3a00001	mov 	r0, 0x1
    b0:	e1a0e00f	mov 	lr, pc
    b4:	e59ff024	ldr 	pc, [pc, 0x24] <- 0x200a06c0
    b8:	e3590000	cmp 	r9,	0x0
    bc:	1a000003	bne	0xd0
    c0:	e3a00000	mov 	r0, 0x0
    c4:	e1a0e00f	mov 	lr, pc
    c8:	e59ff018	ldr 	pc, [pc, 0x18] <- 0x2004f638
    cc:	eaffffcf	b	0x10
    d0:	e28dd020	add 	sp,	sp,	0x20
    d4:	e8bd8fff	ldmia	sp!, {r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, sl, fp, pc}
    d8:	200fea64
    dc:	200330fc
    e0:	200a06c0
    e4:	20054684	andcs 	r4,	r5,	r4 lsl #13
    e8:	2004f638
    ec:	2004fad0
    f0:	0000f81f
    f4:	0000fe5b
    f8:	6c6c6548	copvs
    fc:	0000216f
   100:	74736554	ldrvcb	error 'write back' set
   104:	00000000
   108:	0000aaaa
   10c:	00000000
   110:	0000ffff
