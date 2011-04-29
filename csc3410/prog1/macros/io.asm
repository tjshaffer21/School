; implementation of I/O procedures called by macros in io.obj
; flat memory model version
; author:  R. Detmer
; last revised:  6/98

.386
.MODEL FLAT, syscall

PUBLIC itoaproc, atoiproc, dtoaproc, atodproc, outproc, inproc

.CODE

; itoaproc(source, dest)
; convert integer (source) to string of 6 characters at given destination address
itoaproc    PROC   NEAR32
            push   ebp                  ; save base pointer
            mov    ebp, esp             ; establish stack frame
            push   eax                  ; Save registers
            push   ebx                  ;   used by
            push   ecx                  ;   procedure
            push   edx
            push   edi
            pushf                      ; save flags

            mov    ax, [ebp+12]        ; first parameter (source integer)
            mov    edi, [ebp+8]        ; second parameter (dest offset)
ifSpecial:  cmp    ax,8000h            ; special case -32,768?
            jne    EndIfSpecial        ; if not, then normal case
            mov    BYTE PTR [edi],'-'  ; manually put in ASCII codes
            mov    BYTE PTR [edi+1],'3'  ;   for -32,768
            mov    BYTE PTR [edi+2],'2'
            mov    BYTE PTR [edi+3],'7'
            mov    BYTE PTR [edi+4],'6'
            mov    BYTE PTR [edi+5],'8'
            jmp    ExitIToA            ; done with special case
EndIfSpecial:

            mov    dx, ax              ; save source number

            mov    al,' '              ; put blanks in
            mov    ecx,5               ;   first five
            cld                        ;   bytes of
            rep stosb                  ;   destination field    

            mov    ax, dx              ; copy source number
            mov    cl,' '              ; default sign (blank for +)
IfNeg:      cmp    ax,0                ; check sign of number
            jge    EndIfNeg            ; skip if not negative
            mov    cl,'-'              ; sign for negative number
            neg    ax                  ; number in AX now >= 0
EndIfNeg:

            mov    bx,10               ; divisor

WhileMore:  mov    dx,0                ; extend number to doubleword
            div    bx                  ; divide by 10
            add    dl,30h              ; convert remainder to character
            mov    [edi],dl            ; put character in string
            dec    edi                 ; move forward to next position
            cmp    ax,0                ; check quotient
            jnz    WhileMore           ; continue if quotient not zero

            mov    [edi],cl            ; insert blank or "-" for sign

ExitIToA:   popf                       ; restore flags and registers
            pop    edi
            pop    edx
            pop    ecx
            pop    ebx 
            pop    eax
            pop    ebp 
            ret    6                   ;exit, discarding parameters
itoaproc    ENDP

; dtoaproc(source, dest)
; convert double (source) to string of 11 characters at given offset in DS (dest)
dtoaproc    PROC   NEAR32
            push   ebp                 ; save base pointer
            mov    ebp, esp            ; establish stack frame
            push   eax                 ; Save registers
            push   ebx                 ;   used by
            push   ecx                 ;   procedure
            push   edx
            push   edi
            pushf                      ; save flags

            mov    eax, [ebp+12]         ; first parameter (source double)
            mov    edi, [ebp+8]          ; second parameter (dest addr)
ifSpecialD: cmp    eax,80000000h       ; special case -2,147,483,648?
            jne    EndIfSpecialD       ; if not, then normal case
            mov    BYTE PTR [edi],'-'   ; manually put in ASCII codes
            mov    BYTE PTR [edi+1],'2' ;   for -2,147,483,648
            mov    BYTE PTR [edi+2],'1'
            mov    BYTE PTR [edi+3],'4'
            mov    BYTE PTR [edi+4],'7'
            mov    BYTE PTR [edi+5],'4'
            mov    BYTE PTR [edi+6],'8'
            mov    BYTE PTR [edi+7],'3'
            mov    BYTE PTR [edi+8],'6'
            mov    BYTE PTR [edi+9],'4'
            mov    BYTE PTR [edi+10],'8'
            jmp    ExitDToA            ; done with special case
EndIfSpecialD:

            mov    edx, eax            ; save source number

            mov    al,' '              ; put blanks in
            mov    ecx,10              ;   first ten
            cld                        ;   bytes of
            rep stosb                  ;   destination field    

            mov    eax, edx            ; copy source number
            mov    cl,' '              ; default sign (blank for +)
IfNegD:     cmp    eax,0               ; check sign of number
            jge    EndIfNegD           ; skip if not negative
            mov    cl,'-'              ; sign for negative number
            neg    eax                 ; number in EAX now >= 0
EndIfNegD:

            mov    ebx,10              ; divisor

WhileMoreD: mov    edx,0               ; extend number to doubleword
            div    ebx                 ; divide by 10
            add    dl,30h              ; convert remainder to character
            mov    [edi],dl            ; put character in string
            dec    edi                 ; move forward to next position
            cmp    eax,0               ; check quotient
            jnz    WhileMoreD          ; continue if quotient not zero

            mov    [edi],cl            ; insert blank or "-" for sign

ExitDToA:   popf                       ; restore flags and registers
            pop    edi
            pop    edx
            pop    ecx
            pop    ebx 
            pop    eax
            pop    ebp 
            ret    8                   ;exit, discarding parameters
dtoaproc    ENDP

; atoiproc(source)
; Procedure to scan data segment starting at source address, interpreting 
; ASCII characters as an integer value which is returned in AX.

; Leading blanks are skipped.  A leading - or + sign is acceptable.
; Digit(s) must immediately follow the sign (if any).
; Memory scan is terminated by any non-digit, and the address of 
; the terminating character is in ESI.

; The following flags are affected:
;   AC is undefined
;   PF, SF and ZF reflect sign of number returned in AX.
;   CF reset to 0
;   OF set to indicate error.  Possible error conditions are:
;     - no digits in input
;     - value outside range -32,768 to 32,767
;   (AX) will be 0 if OF is set.

atoiproc    PROC   NEAR32
            push   ebp                 ; save base pointer
            mov    ebp, esp            ; establish stack frame
            sub    esp, 2              ; local space for sign
            push   ebx                 ; Save registers
            push   ecx
            push   edx
            pushf                      ; save flags

            mov    esi,[ebp+8]         ; get parameter (source addr)

WhileBlank: cmp    BYTE PTR [esi],' '  ; space?
            jne    EndWhileBlank       ; exit if not
            inc    esi                 ; increment character pointer
            jmp    WhileBlank          ; and try again
EndWhileBlank:

            mov    ax,1                ; default sign multiplier
IfPlus:     cmp    BYTE PTR [esi],'+'  ; leading + ?
            je     SkipSign            ; if so, skip over
IfMinus:    cmp    BYTE PTR [esi],'-'  ; leading - ?
            jne    EndIfSign           ; if not, save default +
            mov    ax,-1               ; -1 for minus sign
SkipSign:   inc    esi                 ; move past sign
EndIfSign:

            mov    [ebp-2],ax          ; save sign multiplier
            mov    ax,0                ; number being accumulated
            mov    cx,0                ; count of digits so far

WhileDigit: cmp    BYTE PTR [esi],'0'  ; compare next character to '0'
            jl     EndWhileDigit       ; not a digit if smaller than '0'
            cmp    BYTE PTR [esi],'9'  ; compare to '9'
            jg     EndWhileDigit       ; not a digit if bigger than '9'
            imul   ax,10               ; multiply old number by 10
            jo     overflow            ; exit if product too large
            mov    bl,[esi]            ; ASCII character to BL
            and    bx,000Fh            ; convert to single-digit integer
            add    ax,bx               ; add to sum
            jc     overflow            ; exit if sum too large
            inc    cx                  ; increment digit count
            inc    esi                 ; increment character pointer
            jmp    WhileDigit          ; go try next character
EndWhileDigit:

            cmp    cx,0                ; no digits?
            jz     overflow            ; if so, set overflow error flag

; if value is 8000h and sign is '-',  want to return 8000h (-32,768)

            cmp    ax,8000h            ; 8000h ?
            jne    TooBig?
            cmp    WORD PTR [ebp-2],-1 ; multiplier -1 ?
            je     ok1                 ; if so, return 8000h

TooBig?:    test   ax,ax               ; check sign flag
            jns    ok                  ; will be set if number > 32,767

overflow:   pop    ax                  ; get flags
            or     ax,0000100001000100B  ; set overflow, zero & parity flags
            and    ax,1111111101111110B  ; reset sign and carry flags
            push   ax                  ; push new flag values
            mov    ax,0                ; return value of zero
            jmp    AToIExit            ; quit

ok:         imul   WORD PTR [ebp-2]    ; make signed number
ok1:        popf                       ; get original flags
            test   ax,ax               ; set flags for new number
            pushf                      ; save flags

AToIExit:   popf                       ; get flags
            pop    edx                 ; restore registers
            pop    ecx
            pop    ebx
            mov    esp, ebp            ; delete local variable space
            pop    ebp 
            ret    4                   ; exit, removing parameter
atoiproc    ENDP

; atodproc(source)
; Procedure to scan data segment starting at source address, interpreting 
; ASCII characters as an integer value which is returned in EAX.

; Leading blanks are skipped.  A leading - or + sign is acceptable.
; Digit(s) must immediately follow the sign (if any).
; Memory scan is terminated by any non-digit, and the address of 
; the terminating character is in ESI.

; The following flags are affected:
;   AC is undefined
;   PF, SF and ZF reflect sign of number returned in EAX.
;   CF reset to 0
;   OF set to indicate error.  Possible error conditions are:
;     - no digits in input
;     - value outside range -2,147,483,648 to 2,147,483,647
;   (EAX) will be 0 if OF is set.

atodproc    PROC   NEAR32
            push   ebp                 ; save base pointer
            mov    ebp, esp            ; establish stack frame
            sub    esp, 4              ; local space for sign
            push   ebx                 ; Save registers
            push   ecx
            push   edx
            pushf                      ; save flags

            mov    esi,[ebp+8]         ; get parameter (source addr)

WhileBlankD:cmp    BYTE PTR [esi],' '  ; space?
            jne    EndWhileBlankD      ; exit if not
            inc    esi                 ; increment character pointer
            jmp    WhileBlankD         ; and try again
EndWhileBlankD:

            mov    eax,1               ; default sign multiplier
IfPlusD:    cmp    BYTE PTR [esi],'+'  ; leading + ?
            je     SkipSignD           ; if so, skip over
IfMinusD:   cmp    BYTE PTR [esi],'-'  ; leading - ?
            jne    EndIfSignD          ; if not, save default +
            mov    eax,-1              ; -1 for minus sign
SkipSignD:  inc    esi                 ; move past sign
EndIfSignD:

            mov    [ebp-4],eax         ; save sign multiplier
            mov    eax,0               ; number being accumulated
            mov    cx,0                ; count of digits so far

WhileDigitD:cmp    BYTE PTR [esi],'0'  ; compare next character to '0'
            jl     EndWhileDigitD      ; not a digit if smaller than '0'
            cmp    BYTE PTR [esi],'9'  ; compare to '9'
            jg     EndWhileDigitD      ; not a digit if bigger than '9'
            imul   eax,10              ; multiply old number by 10
            jo     overflowD           ; exit if product too large
            mov    bl,[esi]            ; ASCII character to BL
            and    ebx,0000000Fh       ; convert to single-digit integer
            add    eax,ebx             ; add to sum
            jc     overflowD           ; exit if sum too large
            inc    cx                  ; increment digit count
            inc    esi                 ; increment character pointer
            jmp    WhileDigitD         ; go try next character
EndWhileDigitD:

            cmp    cx,0                ; no digits?
            jz     overflowD           ; if so, set overflow error flag

; if value is 80000000h and sign is '-',  want to return 80000000h (-2^32)

            cmp    eax,80000000h       ; 80000000h ?
            jne    TooBigD?
            cmp    DWORD PTR [ebp-4],-1 ; multiplier -1 ?
            je     ok1D                ; if so, return 8000h

TooBigD?:   test   eax,eax             ; check sign flag
            jns    okD                 ; will be set if number > 2^32 - 1

overflowD:  pop    ax                  ; get flags
            or     ax,0000100001000100B  ; set overflow, zero & parity flags
            and    ax,1111111101111110B  ; reset sign and carry flags
            push   ax                  ; push new flag values
            mov    eax,0               ; return value of zero
            jmp    AToDExit            ; quit

okD:        imul   DWORD PTR [ebp-4]   ; make signed number
ok1D:       popf                       ; get original flags
            test   eax,eax             ; set flags for new number
            pushf                      ; save flags

AToDExit:   popf                       ; get flags
            pop    edx                 ; restore registers
            pop    ecx
            pop    ebx
            mov    esp, ebp            ; delete local variable space
            pop    ebp 
            ret    4                   ; exit, removing parameter
atodproc    ENDP

; *************** setup for Win32 I/O ****************

STD_OUTPUT EQU -11
STD_INPUT  EQU -10

GetStdHandle PROTO NEAR32 stdcall,
    nStdHandle:DWORD

ReadFile PROTO NEAR32 stdcall,
    hFile:DWORD, lpBuffer:NEAR32, nNumberOfCharsToRead:DWORD,
    lpNumberOfBytesRead:NEAR32, lpOverlapped:NEAR32

WriteFile PROTO NEAR32 stdcall,
    hFile:DWORD, lpBuffer:NEAR32, nNumberOfCharsToWrite:DWORD,
    lpNumberOfBytesWritten:NEAR32, lpOverlapped:NEAR32

.DATA

written    DWORD  ?
read       DWORD  ?
strAddr    DWORD  ?
strLength  DWORD  ?
hStdOut    DWORD  ?
hStdIn     DWORD  ?

.CODE

; outproc(source)
; Procedure to display null-terminated string
; No registers are changed; flags are not affected.

outproc     PROC   NEAR32
            push   ebp                 ; save base pointer
            mov    ebp, esp            ; establish stack frame
            pushad
            pushfd                     ; save flags

            mov    esi,[ebp+8]         ; source address
            mov    strAddr, esi

; find string length
            mov    strLength, 0        ; initialize string length
WhileChar:  cmp    BYTE PTR [esi], 0   ; character = null?
            jz     EndWhileChar        ; exit if so
            inc    strLength           ; increment character count
            inc    esi                 ; point at next character
            jmp    WhileChar
EndWhileChar:

            INVOKE GetStdHandle,       ; get handle for console output
              STD_OUTPUT
            mov    hStdOut, eax

            INVOKE WriteFile,
              hStdOut,                 ; file handle for screen
              strAddr,                 ; address of string
              strLength,               ; length of string
              NEAR32 PTR written,      ; bytes written
              0                        ; overlapped mode

            popfd                      ; restore flags
            popad                      ; restore registers
            pop    ebp
            ret    4                   ;exit, discarding parameter
outproc     ENDP

; inproc(dest,length)
; Procedure to input a string from keyboard.

; The string will be stored at the address given by dest.

; The length parameter gives the size of the user's buffer.  It is assumed 
; that there will be room for the string and a null byte.
  
; The string will be terminated by a null character (00h).

; Flags are unchanged.

inproc      PROC   NEAR32
            push   ebp                 ; save base pointer
            mov    ebp, esp            ; establish stack frame
            pushad                     ; save all registers
            pushfd                     ; save flags

            INVOKE GetStdHandle,       ; get handle for console
              STD_INPUT
            mov    hStdIn, eax

            mov    ecx, [ebp+8]        ; string length
            mov    strLength, ecx

            mov    esi, [ebp+12]       ; source address
            mov    strAddr, esi

            INVOKE ReadFile,
              hStdIn,                  ; file handle for keyboard
              strAddr,                  ; address of string
              strLength,                ; length of string
              NEAR32 PTR read,          ; bytes read
              0                         ; overlapped mode

            mov    ecx, read           ; number of bytes read
            mov    BYTE PTR [esi+ecx-2],0 ; replace CR/LF by trailing null


            popfd                      ; restore flags
            popad                      ; restore registers
            pop    ebp 
            ret    8                   ; exit, discarding parameters
inproc      ENDP

            END
