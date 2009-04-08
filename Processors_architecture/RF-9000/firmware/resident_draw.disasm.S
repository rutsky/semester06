     0:	e92d4fff	stmdb	sp!, {r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, sl, fp, lr}
     4:	e24dd020	sub 	sp,	sp,	0x20
     8:	e59f50b4	ldr 	r5, [pc, 0xb4] <- 0x200fea64
     c:	e3a06064	mov 	r6, 0x64
    10:	e3a07064	mov 	r7, 0x64
    14:	e2855008	add 	r5,	r5,	0x8
    18:	e1a03005	mov 	r3, r5
    1c:	e3a00000	mov 	r0, 0x0
    20:	e3a01000	mov 	r1, 0x0
    24:	e0040690	mul 	r4, r0, r6
    28:	e0844001	add 	r4,	r4,	r1
    2c:	e59f40bc	ldr 	r4, [pc, 0xbc] <- 0xaaaa
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
    58:	e59ff068	ldr 	pc, [pc, 0x68] <- 0x200330fc
    5c:	e1a09000	mov 	r9, r0
    60:	e3a00001	mov 	r0, 0x1
    64:	e1a0e00f	mov 	lr, pc
    68:	e59ff05c	ldr 	pc, [pc, 0x5c] <- 0x200a06c0
    6c:	e8bd00ff	ldmia	sp!, {r0, r1, r2, r3, r4, r5, r6, r7}
    70:	e3590000	cmp 	r9,	0x0
    74:	1a000010	bne	0xbc
    78:	e3a03005	mov 	r3, 0x5
    7c:	e3a02004	mov 	r2, 0x4
    80:	e59f1050	ldr 	r1, [pc, 0x50] <- 0xf81f
    84:	e59f0050	ldr 	r0, [pc, 0x50] <- 0xfe5b
    88:	e98d000f	stmib	sp, {r0, r1, r2, r3}
    8c:	e28f304c	add 	r3,	pc,	0x4c
    90:	e58d3000	str 	r3, [sp]
    94:	e3a030dc	mov 	r3, 0xdc
    98:	e3a0202e	mov 	r2, 0x2e
    9c:	e3a01000	mov 	r1, 0x0
    a0:	e3a00000	mov 	r0, 0x0
    a4:	e1a0e00f	mov 	lr, pc
    a8:	e59ff020	ldr 	pc, [pc, 0x20] <- 0x20054684
    ac:	e3a00000	mov 	r0, 0x0
    b0:	e1a0e00f	mov 	lr, pc
    b4:	e59ff018	ldr 	pc, [pc, 0x18] <- 0x2004f638
    b8:	eaffffd6	b	0x18
    bc:	e28dd020	add 	sp,	sp,	0x20
    c0:	e8bd8fff	ldmia	sp!, {r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, sl, fp, pc}
    c4:	200fea64
    c8:	200330fc
    cc:	200a06c0
    d0:	20054684
    d4:	2004f638
    d8:	0000f81f
    dc:	0000fe5b
    e0:	6c6c6548	copvs
    e4:	0000216f
    e8:	74736554	ldrvcb	error 'write back' set
    ec:	00000000
    f0:	0000aaaa
