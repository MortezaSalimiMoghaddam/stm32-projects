.text
.global LED
.global LED_off
.global delay
LED:
MOVW r2, 0x0c00
MOVT r2, 0x4002
LDR r3, [r2, #0x0]
MOV r4, #1
LSL r4, #26
ORR r4, r3
STR r4, [r2]
MOV r5, r2
LDR r6, [r5, #0x18]
MOV r7, #1
LSL r7, #13
ORR r7, r6
STR r7, [r5, #0x18]


delay:

LDR r0, =#16800000
loop:
NOP
NOP
NOP
NOP
NOP
NOP
NOP
SUB r0, #1
CMP r0, #0
BNE loop
BX LR


LED_off:
MOVW r2, 0x0c00
MOVT r2, 0x4002
LDR r3, [r2, #0x0]
MOV r4, #1
LSL r4, #26
ORR r4, r3
STR r4, [r2]
MOV r5, r2
LDR r6, [r5, #0x18]
MOV r7, #1
LSL r7, #29
ORR r7, r6
STR r7, [r5, #0x18]
BX LR
