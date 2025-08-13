; ---- test file begins ----
macro NOP
    mov si,2
    this is macro
endm
ASHER: add r3, alon
RONEN: mov si,out
ALON: .data     5 , 6    ,   7    ,  8
.string         "abc"
.entry           ALON
.extern                                  MEIR
    gamma: add si,3
NOP
zzz: nop
add r3, ,4
mov si,3,4
badlabel-1: mov r1,r2
THIS_LABEL_NAME_IS_WAY_TOO_LONG_FOR_THE_RULES: add r1,r2
.data
.data +
.string ""
.string "ok"
.entry 1BAD
.extern   goodName1
labelok:   mov
label2:   add r7
   .data    -1 , 2 , +7
; trailing comment
MOV si,3
.string   "abc def"
.data  10 , , 20

; (empty line below)

; ---- test file ends ----
