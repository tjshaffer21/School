.NOLIST     ; turn off listing
.386

; Obtain the input from the user.
; Preconditions:
;    The prompt that is displayed to the user.
;    The destination, a WORD value, to store the input.
; Postconditions:
;    Over-writes the value of string with the user's input.
;    The input is converted into an integer and stored into ax.
;    The final result is then stored into the given memory location.
getinput    MACRO prompt, dest
   output  prompt         ; prompt
   input   string, 8      ; read ASCII characters
   output  newline
   atoi    string         ; Convert to integer
   mov     dest, ax       ; store into memory
   ENDM


; Subtraction of two coordinates.
; Preconditions:
;    The destination, a WORD value, to store the result.
;    The two coordinates (ie x-coordinate) to do the subtraction.
; Postconditions:
;    ax gets modified to temporarily store the values.
;    The final result is stored within the given WORD memory location.
pointsubpoint    MACRO dest, src, src2
    mov ax, src          ; move source into ax
    sub ax, src2         ; subtract ax and src2
    mov dest, ax         ; mov ax into dest
    ENDM


; Perform the multiplication for dot product.
; Preconditions:
;   The WORD location to store the result.
;   The two values to multiply together.
; Postconditions:
;   ax is used to do calculations.
;   The result is stored within the src2 location
coordproduct    MACRO src, src2
    mov ax, src
    imul src2
    mov src2, ax
    ENDM


; Calculates (1-n)p1-np2/d
; Preconditions:
;  The location to store the result
;  Alpha is assumed to be stored in cx.
;  The two coordinates to use in the equation.
;  The divisor
; Postconditions:
;  ax and bx are used for calculations.
;  Results are stored in dest
calcPi MACRO dest, p1, p2,d
    ; find (1-n)P1
    mov ax, d        ; mov d into ax
    imul p1          ; dp1
    mov bx, ax       ; store bx

    
    mov ax, cx       ; alpha into ax
    imul p2          ; alpha*p2
    
    add ax, bx
    round ax, d
    cwd
    idiv d

    mov dest, ax
    ENDM


; Used to calculate the results inside the square root.
; Preconditions:
;  p1 is the first coordinate to use.
;  pi is the second coordinate to use.
; Postconditions:
;  Uses ax to calculate the results.
;  Stores the result of the calculation in pi.
inSqrt MACRO p1, pi
    mov  ax, p1         ; move p1 into ax
    sub  ax, pi         ; (p1-pi)
    imul ax             ; (p1-pi)^2
    mov  pi, ax         ; store result in pi
    ENDM


; Prints the coordinates as (x,y,z)
; Preconditions:
;    The three word values that are to be printed.
; Postconditions:
;    The string address is used to output the necessary ASCII characters.
printcoord    MACRO x, y, z
    output newline        ; CRLF

    mov    string, '('
    mov    string+1, 0    ; null terminate the output
    output string
    
    itoa   string, x      ; convert to ascii
    output string

    mov string, ','
    mov string+1, 0       ; null terminate the output
    output string

    itoa string,y         ; convert to ascii
    output string

    mov string, ','
    mov string+1, 0       ; null terminate the output
    output string

    itoa string, z        ; convert to ascii
    output string

    mov string, ')'
    mov string+1, 0       ; null terminate the output
    output string

    output newline        ; CRLF
    ENDM

.NOLISTMACRO ; suppress macro expansion listings
.LIST        ; begin listing