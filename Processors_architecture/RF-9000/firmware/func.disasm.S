     0:	e92d4fff	stmdb	sp!, {r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, sl, fp, lr}
     4:	e24ddb07	sub 	sp,	sp,	0x1c00
     8:	e24ddf61	sub 	sp,	sp,	0x184
     c:	eafffffe	b	0xc
    10:	e3a03005	mov 	r3, 0x5
    14:	e3a02001	mov 	r2, 0x1
    18:	e59f1010	ldr 	r1, [pc, 0x10] <- 0xf81f
    1c:	e59f0010	ldr 	r0, [pc, 0x10] <- 0xfe5b
    20:	e28ddb07	add 	sp,	sp,	0x1c00
    24:	e28ddf65	add 	sp,	sp,	0x194
    28:	e8bd8ff0	ldmia	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
    2c:	000fa600	andeq 	sl,	pc,	r0 lsl #12
    30:	0000f81f
    34:	0000fe5b
    38:	6c6c6548	copvs
    3c:	6f77206f	swivs
    40:	21646c72	cmncs 	??????????????
    44:	21646c0a	cmncs 	??????????????
