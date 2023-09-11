.text
.global LED_on
.global Delay
.global LED_off

LED_on:

MOVW r2, 0x0800
MOVT r2, 0x4001
LDR r3, [r2, #0x0]
MOV r4, #1
LSL r4, #4
ORR r4, r3
STR r4, [r2]
MOV r5, r2
LDR r6, [r5, #0x10]
MOV r7, #1
LSL r7, #1
ORR r7, r6
STR r7, [r5, #0x10]

Delay:

LDR r0, = #7200000
loop:
SUB r0, #1
CMP r0, #0
BNE loop
BX LR


LED_off:

MOVW r2, 0x0800
MOVT r2, 0x4001
LDR r3, [r2, #0x0]
MOV r4, #1
LSL r4, #4
ORR r4, r3
STR r4, [r2]
MOV r5, r2
LDR r6, [r5, #0x10]
MOV r7, #1
LSL r7, #17
ORR r7, r6
STR r7, [r5, #0x10]
BX LR
