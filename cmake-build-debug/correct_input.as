;demo cmd using a label defined after the cmd
JOKE:    .entry R
JOKE:    call R
NOTdefined: call label2
beq $1, $2, NOTdefined

;demo 31-char label
Next123456789123456778922385461: move $20,$4

;demo r cmd
label1: add $5, $2, $31
;demo conditional cmd referring to a pre-defined label
;and another that refers to a label that hadn't yet been defined
beq $2, $9, label1
beq $23, $16, label2
label2: stop

;demo extern not used (therefore no .ext file)
;also demo the same label defined twice as extern
H:    .extern val3
a:    .extern val3
h:    .entry label1
n:   .entry label1

;demo all data types
    .asciz "this file demonstrates correct input and execution"
    .asciz "I hope..."
    .asciz "Some number, because why not? Hope you like pi"
label3: .db 3,1,4,1,59,26,5,38,97,9
label1000: .dw 31415,9,265,3897,9
labelQ: .dh -3,1,4,-59,+265

;demo different cmds
addi $9, -45, $8
jmp label1
jmp $23
stop

    .asciz "More entries. Let's make a long .ent file."

    .entry a
    .entry c
    .entry h
    .entry m
    .entry a
    .entry n
    .entry i
    .entry n
    .entry o
    .entry v

    .entry H
    .entry i
    .entry l
    .entry a
    .entry r
    .entry y

    .entry H
    .entry a
    .entry h
    .entry n

R: la label1
a: sub $3,$31, $0
c: .asciz "Is a great composer."
h: .asciz "And Hilary is a remarkable violinist."
m:.dh 0,-601,1700, 2
n:.db 7,-57
i: .asciz "You should listen to her playing brahms :) ."
o: blt $5, $24, R
v: mvhi $8, $5
H: mvhi $8, $5
l: mvhi $8, $5
r: mvhi $8, $5
y: mvhi $8, $5


