and $10 , $0 , $22

mvlo $12 , $15

blt $28 , $11, loop

stop

sw $15 , 19, $4

or $27 , $7 , $11

sb $29 , +9,$16

la loop

;bne $29 , $12 - label must start with instead of missing param

nor $28 , $4 , $18

lw $16 ,5, $29

sw $16 ,-3, $13

la hi

addi $24 ,4, $4

lh $3 ,4, $12

;bgt $2 , $9

mvlo $1 , $0

;ori $6 , $4

;addi $29 , $22

;nori $2 , $18

;lw $2 , $7

lb $17,8 , $31

add $29 , $14 , $5

la hi

stop

mvlo $1 , $21

andi $2 , 4,$2

or $0 , $10 , $11

move $5 , $20

and $10 , $0 , $22
;error in prev line because the line doesn't end with \n
;if the program ends without cmd stop -- error???