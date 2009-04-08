     0:	e92d4fff	stmdb	sp!, {r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, sl, fp, lr}
     4:	e24dd020	sub 	sp,	sp,	0x20
     8:	e59f50a8	ldr 	r5, [pc, 0xa8] <- 0x200fea64
     c:	e3a06064	mov 	r6, 0x64
    10:	e3a07064	mov 	r7, 0x64
    14:	e2855008	add 	r5,	r5,	0x8
    18:	e1a03005	mov 	r3, r5
    1c:	e3a00000	mov 	r0, 0x0
    20:	e3a01000	mov 	r1, 0x0
    24:	e0040690	mul 	r4, r0, r6
    28:	e0844001	add 	r4,	r4,	r1
    2c:	e59f40ac	ldr 	r4, [pc, 0xac] <- 0xaaaa
    30:	e1c340b0	strh	r4, [r3, 0x0]
    34:	e2833002	add 	r3,	r3,	0x2
    38:	e2811001	add 	r1,	r1,	0x1
    3c:	e1510006	cmp 	r1,	r6
    40:	bafffff7	blt	0x24
    44:	e2800001	add 	r0,	r0,	0x1
    48:	e1500007	cmp 	r0,	r7
    4c:	bafffff3	blt	0x20
    50:	e92d00ff	stmdb	sp!, {r0, r1, r2, r3, r4, r5, r6, r7}
    54:	e1a0e00f	mov 	lr, pc
    58:	e59ff05c	ldr 	pc, [pc, 0x5c] <- 0x200330fc
    5c:	e1a09000	mov 	r9, r0
    60:	e3a00001	mov 	r0, 0x1
    64:	e1a0e00f	mov 	lr, pc
    68:	e59ff050	ldr 	pc, [pc, 0x50] <- 0x200a06c0
    6c:	e8bd00ff	ldmia	sp!, {r0, r1, r2, r3, r4, r5, r6, r7}
    70:	e3590000	cmp 	r9,	0x0
    74:	1a00000d	bne	0xb0
    78:	e3a03005	mov 	r3, 0x5
    7c:	e3a02004	mov 	r2, 0x4
    80:	e59f1040	ldr 	r1, [pc, 0x40] <- 0xf81f
    84:	e59f0040	ldr 	r0, [pc, 0x40] <- 0xfe5b
    88:	e98d000f	stmib	sp, {r0, r1, r2, r3}
    8c:	e28f303c	add 	r3,	pc,	0x3c
    90:	e58d3000	str 	r3, [sp]
    94:	e3a030dc	mov 	r3, 0xdc
    98:	e3a0202e	mov 	r2, 0x2e
    9c:	e3a01000	mov 	r1, 0x0
    a0:	e3a00000	mov 	r0, 0x0
    a4:	e1a0e00f	mov 	lr, pc
    a8:	e59ff014	ldr 	pc, [pc, 0x14] <- 0x20054684
    ac:	eaffffd9	b	0x18
    b0:	e28dd020	add 	sp,	sp,	0x20
    b4:	e8bd8fff	ldmia	sp!, {r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, sl, fp, pc}
    b8:	200fea64
    bc:	200330fc
    c0:	200a06c0
    c4:	20054684
    c8:	0000f81f
    cc:	0000fe5b
    d0:	6c6c6548	copvs
    d4:	0000216f
    d8:	74736554	ldrvcb	error 'write back' set
    dc:	00000000
    e0:	0000aaaa
