.syntax unified
.arch armv7-a

.equ STACK, 0x10000000
/*
mov r0, #4
lsl r0, r0, #16
ldr r1, =0xaaaaaaaa
str r1, [r0]
bx lr
*/

jmp_mem2:
mov r0, #0x12
lsl r0, r0, #24
add r0, r0, #0x10
bx r0

_start:	
push {r4-r11, lr}

mov	r0, #2
mcr	p14, 7, r0, c1, c0, 0
mov	r0, #1
mcr	p14, 7, r0, c2, c0, 0

mov r6, #STACK
mov r0, #8
lsl r0, r0, 16
add r7, r6, r0 // local vars

ldr r5, =addr_handler_table
ldr r5, [r5]

ldr r12, =addr_exit_jazelle
ldr r12, [r12]

ldr lr, =addr_jvm_bytecode
ldr lr, [lr]

// entering jazelle mode

bxj r12

.align 4

addr_handler_table:
.4byte handler_table
addr_exit_jazelle:
.4byte exit_jazelle
addr_jvm_bytecode:
.4byte jvm_bytecode

exit_jazelle:
mov	r0, #0
mcr	p14, 7, r0, c1, c0, 0
mcr	p14, 7, r0, c2, c0, 0

mov r0, #0x13
lsl r0, r0, #24
add r0, r0, #4
str r6, [r0]
add r0, r0, #4
str r6, [r0]
sub r0, r0, #8
mov r1, #0xee
lsl r1, r1, #8
add r1, #0xee
lsl r1, r1, #8
add r1, #0xee
lsl r1, r1, #8
add r1, #0xee
str r1, [r0]

pop {r4-r11, pc}

bx lr

handler_table:
.include "src/arm/table.s"
.include "src/arm/instr_handlers.s"
.include "src/arm/native_instrs.s"
.include "src/arm/exceptions.s"

.word 0xabcdefaa
.word 4
jvm_bytecode:
.incbin "bytecode/bytecode"
