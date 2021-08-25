;blt  , $3 , label
;move $13 , $13
;sw $7 , 45 , $19
;bgt $14 , $17 , label
;call $4
;and myVar , $12 , $25
;sh $7 , 45 , $7
;sh $21 , 45 , $12
;move myVar , $16
;sw myVar , 45 , $30
;call $6
;sw $3 , 45 , $12
;and $7 , $11 , $6
;and $14 , $16 , $6
;nor myVar , $27 , $1
;add $8 , $22 , $27
;jmp $24
;jmp $3
;bne $26 , $1 , label
;la $4
;call $21
;addi $6 , 45 , $0
;sw myVar , 45 , $5 - return expecting an interger rather than excpecting a reg
;sw $31 , 45 , $29
;la $31
;subi 45 , 45 , $19
;nori $12 , 45 , $22
;nor $32 , $12 , $0
;sb $16 , 45 , $20
;sh $14 , 45 , $5
;lw $24 , 45 , $12
;ori $29 , 45 , $20
;bne label , $26 , label
;nori $17 , 45 , $21
;move $24 , $31
;jmp $0
;sub $20 , $0 , $2
;ori  , 45 , $30
;sh $12 , 45 , $30
;mvlo $8 , $13
;move $  , $6
;blt $4 , $25 , label
;add $23 , $23 , $3
;blt label , $30 , label
;add $12 , $13 , $11
;sb $2 , 45 , $31
;blt $26 , $3 , label
;mvhi $31 , $27
;sb $22 , 45 , $17
;sh $12 , 45 , $28
lh  , 45 , $2
blt $3 , $25 , label 
add $18 , $6 , $32 
lh 45 , 45 , $15 
nori $25 , 45 , $24 
or $11 , $16 , $26 
andi $27 , 45 , $23 
ori $27 , 45 , $23 
sh $6 , 45 , $21 
