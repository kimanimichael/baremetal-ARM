
main.bin:     file format binary


Disassembly of section .data:

00000000 <.data>:
   0:	0000      	movs	r0, r0
   2:	2003      	movs	r0, #3
   4:	0009      	movs	r1, r1
   6:	0800      	lsrs	r0, r0, #32
   8:	b580      	push	{r7, lr}
   a:	b082      	sub	sp, #8
   c:	af00      	add	r7, sp, #0
   e:	4b14      	ldr	r3, [pc, #80]	; (0x60)
  10:	681b      	ldr	r3, [r3, #0]
  12:	607b      	str	r3, [r7, #4]
  14:	4b13      	ldr	r3, [pc, #76]	; (0x64)
  16:	681b      	ldr	r3, [r3, #0]
  18:	603b      	str	r3, [r7, #0]
  1a:	e007      	b.n	0x2c
  1c:	687a      	ldr	r2, [r7, #4]
  1e:	1d13      	adds	r3, r2, #4
  20:	607b      	str	r3, [r7, #4]
  22:	683b      	ldr	r3, [r7, #0]
  24:	1d19      	adds	r1, r3, #4
  26:	6039      	str	r1, [r7, #0]
  28:	6812      	ldr	r2, [r2, #0]
  2a:	601a      	str	r2, [r3, #0]
  2c:	4b0e      	ldr	r3, [pc, #56]	; (0x68)
  2e:	681b      	ldr	r3, [r3, #0]
  30:	683a      	ldr	r2, [r7, #0]
  32:	429a      	cmp	r2, r3
  34:	d3f2      	bcc.n	0x1c
  36:	4b0d      	ldr	r3, [pc, #52]	; (0x6c)
  38:	681b      	ldr	r3, [r3, #0]
  3a:	603b      	str	r3, [r7, #0]
  3c:	e004      	b.n	0x48
  3e:	683b      	ldr	r3, [r7, #0]
  40:	1d1a      	adds	r2, r3, #4
  42:	603a      	str	r2, [r7, #0]
  44:	2200      	movs	r2, #0
  46:	601a      	str	r2, [r3, #0]
  48:	4b09      	ldr	r3, [pc, #36]	; (0x70)
  4a:	681b      	ldr	r3, [r3, #0]
  4c:	683a      	ldr	r2, [r7, #0]
  4e:	429a      	cmp	r2, r3
  50:	d3f5      	bcc.n	0x3e
  52:	f000 f81f 	bl	0x94
  56:	bf00      	nop
  58:	3708      	adds	r7, #8
  5a:	46bd      	mov	sp, r7
  5c:	bd80      	pop	{r7, pc}
  5e:	bf00      	nop
  60:	00e8      	lsls	r0, r5, #3
  62:	0800      	lsrs	r0, r0, #32
  64:	0000      	movs	r0, r0
  66:	2000      	movs	r0, #0
  68:	0000      	movs	r0, r0
  6a:	2000      	movs	r0, #0
  6c:	0000      	movs	r0, r0
  6e:	2000      	movs	r0, #0
  70:	0000      	movs	r0, r0
  72:	2000      	movs	r0, #0
  74:	b480      	push	{r7}
  76:	b083      	sub	sp, #12
  78:	af00      	add	r7, sp, #0
  7a:	6078      	str	r0, [r7, #4]
  7c:	bf00      	nop
  7e:	687b      	ldr	r3, [r7, #4]
  80:	1e5a      	subs	r2, r3, #1
  82:	607a      	str	r2, [r7, #4]
  84:	2b00      	cmp	r3, #0
  86:	d1fa      	bne.n	0x7e
  88:	bf00      	nop
  8a:	bf00      	nop
  8c:	370c      	adds	r7, #12
  8e:	46bd      	mov	sp, r7
  90:	bc80      	pop	{r7}
  92:	4770      	bx	lr
  94:	b580      	push	{r7, lr}
  96:	af00      	add	r7, sp, #0
  98:	4b0f      	ldr	r3, [pc, #60]	; (0xd8)
  9a:	681b      	ldr	r3, [r3, #0]
  9c:	4a0e      	ldr	r2, [pc, #56]	; (0xd8)
  9e:	f043 0302 	orr.w	r3, r3, #2
  a2:	6013      	str	r3, [r2, #0]
  a4:	4b0d      	ldr	r3, [pc, #52]	; (0xdc)
  a6:	681b      	ldr	r3, [r3, #0]
  a8:	4a0c      	ldr	r2, [pc, #48]	; (0xdc)
  aa:	f443 4380 	orr.w	r3, r3, #16384	; 0x4000
  ae:	6013      	str	r3, [r2, #0]
  b0:	4b0b      	ldr	r3, [pc, #44]	; (0xe0)
  b2:	681b      	ldr	r3, [r3, #0]
  b4:	4a0a      	ldr	r2, [pc, #40]	; (0xe0)
  b6:	f043 0380 	orr.w	r3, r3, #128	; 0x80
  ba:	6013      	str	r3, [r2, #0]
  bc:	4809      	ldr	r0, [pc, #36]	; (0xe4)
  be:	f7ff ffd9 	bl	0x74
  c2:	4b07      	ldr	r3, [pc, #28]	; (0xe0)
  c4:	681b      	ldr	r3, [r3, #0]
  c6:	4a06      	ldr	r2, [pc, #24]	; (0xe0)
  c8:	f023 0380 	bic.w	r3, r3, #128	; 0x80
  cc:	6013      	str	r3, [r2, #0]
  ce:	4805      	ldr	r0, [pc, #20]	; (0xe4)
  d0:	f7ff ffd0 	bl	0x74
  d4:	e7ec      	b.n	0xb0
  d6:	bf00      	nop
  d8:	3830      	subs	r0, #48	; 0x30
  da:	4002      	ands	r2, r0
  dc:	0400      	lsls	r0, r0, #16
  de:	4002      	ands	r2, r0
  e0:	0414      	lsls	r4, r2, #16
  e2:	4002      	ands	r2, r0
  e4:	4240      	negs	r0, r0
  e6:	000f      	movs	r7, r1
