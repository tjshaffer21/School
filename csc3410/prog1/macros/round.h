.NOLIST     ; turn off listing
.386

EXTRN round_proc:Near32

; the round macro assumes that dividend and divisor are WORDs
; the corrected dividend is in ax

round       	MACRO   dividend, divisor
                   push dividend
                   push divisor
                   call round_proc
                ENDM

.NOLISTMACRO ; suppress macro expansion listings
.LIST        ; begin listing