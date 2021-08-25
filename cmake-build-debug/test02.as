;file ps.as
;sample source code


.entry Next
.extern wNumber
;problems with cmds:

STR: .asciz "aBcd"
    .dw 1,2,3,4,5,6,7,8,9,10,1,2,3,4,5,6,7,8,9,10,1,2,3,4,5,6,7,8,9,10,1,2,3,4,5,6,7,8,9,10,1,2,3,4,5,6,7,8,9,10,1,2,3,4,5,6,7,8,9,10,1,2,3,4,5,6,7,8,9,10
MAIN: add $3, $5,$9
LOOP: ori $9,-5,$2
la val1
jmp Next
Next: move $20,$4
LIST: .db 6,-9
bgt $4,$2,END
la K
sw $0,4,$10
bne $31,$9, LOOP
call val1
jmp $4
la wNumber
.extern val1
.dh 27056
K:.dw 31,-12
END:stop
.entry K

;STR: .asciz "aBcd"
;LIST: .db 6,-9
;      .dh 27056
;.entry K
;K: .dw 31,-12
;	la wNumber
;.extern val1
;	.dh 27056
;K: .dw 31,-12
;END stop
;.entry K