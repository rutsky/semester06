     0:	e92d4fff	stmdb	sp!, {r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, sl, fp, lr}
     4:	e24dd020	sub 	sp,	sp,	0x20
     8:	e3a03005	mov 	r3, 0x5
     c:	e3a02004	mov 	r2, 0x4
    10:	e59f1030	ldr 	r1, [pc, 0x30] <- 0xf81f
    14:	e59f0030	ldr 	r0, [pc, 0x30] <- 0xfe5b
    18:	e98d000f	stmib	sp, {r0, r1, r2, r3}
    1c:	e28f302c	add 	r3,	pc,	0x2c
    20:	e58d3000	str 	r3, [sp]
    24:	e3a030dc	mov 	r3, 0xdc
    28:	e3a0202e	mov 	r2, 0x2e
    2c:	e3a01000	mov 	r1, 0x0
    30:	e3a00000	mov 	r0, 0x0
    34:	e1a0e00f	mov 	lr, pc
    38:	e59ff004	ldr 	pc, [pc, 0x4] <- 0x20054684
    3c:	e28dd020	add 	sp,	sp,	0x20
    40:	e8bd8fff	ldmia	sp!, {r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, sl, fp, pc}
    44:	20054684
    48:	0000f81f
    4c:	0000fe5b
    50:	6c6c6548	copvs
    54:	57202c6f	strpl	r2, [r0, -pc ror #24]!
    58:	646c726f	strvsb	error 'write back' set
    5c:	64000021	strvs	r0, [r0], -0x21
