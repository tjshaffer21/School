;
; Author:       Thomas Shaffer
; Program:      distance.asm
; Date:         7 October 2009
; Description:  Computers the distance between a point and a plane.
;

.386
.MODEL FLAT

ExitProcess PROTO NEAR32 stdcall, dwExitCode:DWORD

INCLUDE macros/debug.h
INCLUDE macros/round.h
INCLUDE macros/sqrt.h
INCLUDE macros/macros.h

CR    EQU   0dh     ; Carriage Return character
LF    EQU   0ah     ; Line Feed character

.STACK 4096

.DATA
xn          WORD    ?    ; x of normal
yn          WORD    ?    ; y of normal
zn          WORD    ?    ; z of normal
xp          WORD    ?    ; x on plane
yp          WORD    ?    ; y on plane
zp          WORD    ?    ; z on plane
xnp         WORD    ?    ; x not on plane
ynp         WORD    ?    ; y not on plane
znp         WORD    ?    ; z not on plane
xnp2        WORD    ?    ; second x not on plane
ynp2        WORD    ?    ; second y not on plane
znp2        WORD    ?    ; second z not on plane
xa          WORD    ?
ya          WORD    ?
za          WORD    ?
xa2         WORD    ?
ya2         WORD    ?
za2         WORD    ?
prompt1     BYTE    "Enter the x-coordinate of the normal of the plane: ", 0
prompt2     BYTE    "Enter the y-coordinate of the normal of the plane: ", 0
prompt3     BYTE    "Enter the z-coordinate of the normal of the plane: ", 0
prompt4     BYTE    "Enter the x-coordinate of the point on the plane: ", 0
prompt5     BYTE    "Enter the y-coordinate of the point on the plane: ", 0
prompt6     BYTE    "Enter the z-coordinate of the point on the plane: ", 0
prompt7     BYTE    "Enter the x-coordinate of the point not on the plane: ", 0
prompt8     BYTE    "Enter the y-coordinate of the point not on the plane: ", 0
prompt9     BYTE    "Enter the z-coordinate of the point not on the plane: ", 0
prompt10    BYTE    "The point of intersection is: ", 0
prompt11    BYTE    "The distance from point to the plane is: ", 0
string      BYTE    13  DUP (?)
newline     BYTE    CR, LF, 0

.CODE
_start:
   ; Obtain the three sets of input from the user to do calculations.
   getinput    prompt1, xn
   getinput    prompt2, yn
   getinput    prompt3, zn
   printcoord  xn, yn, zn

   getinput    prompt4, xp
   getinput    prompt5, yp
   getinput    prompt6, zp
   printcoord  xp, yp, zp   

   getinput    prompt7, xnp
   getinput    prompt8, ynp
   getinput    prompt9, znp
   printcoord  xnp, ynp, znp

   ; Obtain a second point not on the plane.
   mov ax, xn        ; Store the x normal in ax.
   add ax, xnp       ; Add the x of point not on plane to xn.
   mov xnp2, ax      ; Store the answer in xnp2.

   mov ax, yn        ; Store the y normal in ax.
   add ax, ynp       ; Add the y of point not on plane to yn.
   mov ynp2, ax      ; Store the answer in ynp2.

   mov ax, zn        ; Store the z normal in ax.
   add ax, znp       ; Add the z of point not on plane to zn.
   mov znp2, ax      ; Store the answer in znp2.

   ; (P0-P1) - stored within the ?a values.
   pointsubpoint xa, xp, xnp
   pointsubpoint ya, yp, ynp
   pointsubpoint za, zp, znp

   ; n.(P0-P1) - use dot
   ; result stored in ?a
   coordproduct xn, xa
   coordproduct yn, ya
   coordproduct zn, za

   ; Combine the terms together.
   mov ax, xa                 ; mov xa into ax.
   add ax, ya
   add ax, za
   mov xa, ax                 ; Store n.(P1-P0) into xa.


   ; (P2-P1) - stored within the ?a2 values.
   pointsubpoint xa2, xnp2, xnp
   pointsubpoint ya2, ynp2, ynp
   pointsubpoint za2, znp2, znp

   ; n.(P1-P2)
   coordproduct xn, xa2
   coordproduct yn, ya2
   coordproduct zn, za2

   ; Combine the terms together.
   mov ax, xa2                ; mov xa2 into ax
   add ax, ya2                
   add ax, za2
   mov xa2, ax                ; Store n.(P1-P2) into xa2
  
   ; pi = (1-n)p1-np2/d
   mov     cx, xa                ; Store alpha in cx
   calcPi  xa, xnp, xn, xa2    ; Calculate Pix and store in xa
   calcPi  ya, ynp, yn, xa2    ; Calculate Piy and store in ya
   calcPi  za, znp, zn, xa2    ; Calculate Piz and store in za

   ; Display the intersection.
   output prompt10
   output newline
   printcoord xa, ya, za
 
   ; Calculate the distance
   inSqrt xnp, xa
   inSqrt ynp, ya
   inSqrt znp, za

   ; Add the inner values together.
   mov ax, xa
   add ax, ya
   add ax, za

   ; Take the square root.
   sqrt ax

   ; Output the distance
   output prompt11
   outputW ax

   INVOKE ExitProcess, 0        ; Exit with vaxue of 0

PUBLIC _start
END
