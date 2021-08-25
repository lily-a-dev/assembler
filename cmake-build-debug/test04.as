;ok

;.dw 1,2,3,4,5,6,7,8,9,10,1,2,3,4,5,6,7,8,9,10,1,2,3,4,5,6,7,8,9,10,1,2,3,4,5,3
;.dw 1,2,3,4,5,6,7,8,9,10,1,2,3,4,5,6,7,8,9,10,1,2,3,4,5,6,7,8,9,10,1,2,3,4,5,64
;.db 7,-57,17,+9
;.dw 13352345234522345
;.asciz "hello what's up " this is rad "
;.asciz " "
;hi:.asciz "\n"
;.extern hi
;
;
;add   $3, $5,$9
;add 	$3, $5,$9
;add $3	, $5,$9
;add $3, $5  ,  $9
;	add   $3, $5,$9
;  add   $3,$5,$9
;
;
;Next4: move $20,$4
;net: move $20,$4
;b1241340: move $20,$4
;n345k: move $20,$4
;goodLabel: and $2, $2, $3
;


;undefined cmd
;add5
;Add
;ADD
;sab
;.add
;ad d
;5add
;DB
;db
;
;
;value out of range
;
;not an int
;.db 4, a, 1
;.dw "2423"
;.db 7,-57,17,++9
;.dw +-13093345
;.db a7,-57,17,+9
;.dw 345, 130.98
;.dw 345, 130.98, 132
;.db 7,--57,17,+9
;.dw $,135345
;.db 7, a,17,+9
;
;
;
;missing param
;add
;jmp
;addi $9, $23
;subi $3, -98
;ori -3, $12
;nori 2
;blt $2, notDefined
;beq $1, $2
;bgt notDefined
;beq $1, $2,
;lh $9, $23
;sw $3, -98
;lb -3, $12
;lw 2
;la
;.asciz
;.db

; extranous text
;add $3, $5, $9, $21
;add $3, $5, $9 k
;add $3, $5, $9k
;call main $2
;call $2 main
;beq $12, $3, loop,
;beq $12, $3, loop 23
;END: stop 345
;stop 234
;.asciz "hello" oij
;    .dw 345, 13098,

;conscutive comma
;add $3, $5 , , $9
;add $3,, $5, $9
;.dh 4a
;.db 7,-57,17, ,9
;.db 7,-57, ,17,+9
;.dw 345, 1308,,+132
;

; missing comma
;add $3, $5 $9
;add $3, $5 	$9
;add $3 $5 $9
;.dw 135345 8
;.db 7 +9
;.db 7 -9
;.db +7 -9
;.dh 4a
;add $3a, $5, $9

; illegal comma
;add , $3, $5 $9
;add, $3, $5 , $9
;.db , 7 +9
;.db 7, +9,
;call, val1
;.entry, tree
;.extern, wood
;.asciz, "hello"


;input exceeding max length
;.dw 1,2,3,4,5,6,7,8,9,10,1,2,3,4,5,6, 7,8,9,1
;0,1,2,3,4,5,6,7,8,9,10,1,2,3,4,5,6,7,8,9,10, 1,2,3,4,
    ;5,6,7,8,9,10,1,2,3,4,5,6,7,8,9,10,1,2,3,4,5,6,7,8,9,10


; reg must start with $
;add $3, 5 , $9

;not a reg
;add 3, $5, $9
;add $3, 5, $9
;add $3, $a, $9

;str must terminate with "
;.asciz "hello kjhkjh867876&
;. asciz "sd"

; illegal space between $ and num
;add $3, $ 5 , $9

; reg out of bounds
;add $3, $32 , $9
;add $3, $30 , $-1
;

;label defined more than once
;good: stop
;good: call hi

;illegal space before colon
;Next3 : move $20,$4
;Next4	: move $20,$4
;next5 : move $20,$4
;LIST2 : .db 6,-9

;label must start with alpha
;3LIST2: .db 6,-9
;_Next4: move $20,$4
;blt $2, $30, 2loop
;beq $2, $30, _loop
;call 3val
;jmp _ks

;label only alphanum
;N.ext4: move $20,$4
;Next#4: move $20,$4
;Next_4: move $20,$4
;jmp _ks
;jmp k_s
;blt $2, $30, 2&loop
;beq $2, $30, 2.loop


;label cannot be a saved word
;add: move $20,$4
;extern: move $20,$4
;entry: move $20,$4
;dw: move $20,$4
;dh: move $20,$4
;sub: move $20,$4
;jmp add
;blt $2, $30, extern
;beq $2, $30, .entry
;
;ok labels
;aDd: move $20,$4
;DB: move $20,$4
;ENTRY: move $20,$4

;label already defined
;ONCE: sw $0, 4, $10
;ONCE: sw $0, 4, $10


;label too long
;theyre legal but i get an error
;Next1234567891234567789223385461: move $20,$4
;Next123456789123456778922385461: move $20,$4
;beq $2, $30, Next12345678912345677892238546we
;jmp addkjhgslkdfjhgsklfhgslkdssdfgs2

;undefined instruct
;.skj
;.23
;.ASCIZ
;.hello
;.ENTRY
;.EXTERNAL


;illegal space between dot and instruct
;. db "hello"
;. asciz "hello"
;. entry hello
;. external bye


;label not prev defined - second pass
;blt $2, $30, notDefined
;beq $1, $2, NOTdefined

;label already ext
;.extern val3
;.entry val3
;.entry val3
;.extern val3


;
;label cannot be ext
;    .extern out
;blt $6, $7, out
;

;undefined coding cmd
;Next6 move $20,$4


; expecting n ops
;add $3, $9
;move $23, $2, $9
;mvhi $2
;mvlo

;.extern out
;    .extern goodLabel
;la out
;jmp out
;blt $2, $30, goodLabel

;illegal characters in str
;    .asciz "לדחגכ"
;ND:stop
;.extern g
;call g

;test immd works for forwards and backwards addresses
label1: add $5, $2, $31
beq $2, $9, label1
beq $23, $16 label2
label2: stop