	.section	__TEXT,__text,regular,pure_instructions
	.build_version macos, 26, 0	sdk_version 26, 2
	.globl	_main                           ; -- Begin function main
	.p2align	2
_main:                                  ; @main
	.cfi_startproc
; %bb.0:
	sub	sp, sp, #64
	stp	x29, x30, [sp, #48]             ; 16-byte Folded Spill
	add	x29, sp, #48
	.cfi_def_cfa w29, 16
	.cfi_offset w30, -8
	.cfi_offset w29, -16
	sub	x8, x29, #4
	mov	w9, #772                        ; =0x304
	movk	w9, #258, lsl #16
	stur	w9, [x29, #-4]
	stur	x8, [x29, #-16]
	ldur	x8, [x29, #-16]
	ldrb	w13, [x8]
	ldur	x8, [x29, #-16]
	ldrb	w12, [x8, #1]
	ldur	x8, [x29, #-16]
	ldrb	w11, [x8, #2]
	ldur	x8, [x29, #-16]
	ldrb	w10, [x8, #3]
	mov	x9, sp
                                        ; implicit-def: $x8
	mov	x8, x13
	str	x8, [x9]
                                        ; implicit-def: $x8
	mov	x8, x12
	str	x8, [x9, #8]
                                        ; implicit-def: $x8
	mov	x8, x11
	str	x8, [x9, #16]
                                        ; implicit-def: $x8
	mov	x8, x10
	str	x8, [x9, #24]
	adrp	x0, l_.str@PAGE
	add	x0, x0, l_.str@PAGEOFF
	bl	_printf
	mov	w0, #0                          ; =0x0
	ldp	x29, x30, [sp, #48]             ; 16-byte Folded Reload
	add	sp, sp, #64
	ret
	.cfi_endproc
                                        ; -- End function
	.section	__TEXT,__cstring,cstring_literals
l_.str:                                 ; @.str
	.asciz	"%02x %02x %02x %02x\n"

.subsections_via_symbols
