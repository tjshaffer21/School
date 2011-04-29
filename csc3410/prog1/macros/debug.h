INCLUDE io.h

CR          EQU    0Dh   ; carriage return character
LF          EQU    0Ah   ; linefeed character

.DATA
carriage    BYTE     CR, LF, 0
text        BYTE     13 DUP(?)

outputW    	MACRO   var
                   itoa text, var
                   output text
                   output carriage
                ENDM

outputD  	MACRO  var
                   dtoa text, var
                   output text
                   output carriage
         	ENDM

inputW          MACRO  prompt, location
                   output prompt
                   input text, 8
                   atoi text
                   mov location, ax
                ENDM

inputD          MACRO  prompt, location
                   output prompt
                   input text, 13
                   atod text
                   mov location, eax
                ENDM