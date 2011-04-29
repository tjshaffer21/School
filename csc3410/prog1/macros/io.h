; IO.H -- header file for I/O macros
; 32-bit version for flat memory model
; R. Detmer   last revised 8/2000
.NOLIST     ; turn off listing
.386

            EXTRN  itoaproc:near32, atoiproc:near32
            EXTRN  dtoaproc:near32, atodproc:near32
            EXTRN  inproc:near32, outproc:near32

itoa        MACRO  dest,source,xtra    ;; convert integer to ASCII string

            IFB    <source>
            .ERR <missing operand(s) in ITOA>
            EXITM
            ENDIF

            IFNB   <xtra>
            .ERR <extra operand(s) in ITOA>
            EXITM
            ENDIF

            push   ebx                  ;; save EBX
            mov    bx, source
            push   bx                   ;; source parameter
            lea    ebx,dest             ;; destination address
            push   ebx                  ;; destination parameter
            call   itoaproc             ;; call itoaproc(source,dest)
            pop    ebx                  ;; restore EBX
            ENDM

atoi        MACRO  source,xtra          ;; convert ASCII string to integer in AX
                                        ;; offset of terminating character in ESI

            IFB    <source>
            .ERR <missing operand in ATOI>
            EXITM
            ENDIF

            IFNB   <xtra>
            .ERR <extra operand(s) in ATOI>
            EXITM
            ENDIF

            push   ebx                 ;; save EBX
            lea    ebx,source          ;; source address to EBX
            push   ebx                 ;; source parameter on stack
            call   atoiproc            ;; call atoiproc(source)
            pop    ebx                 ;; parameter removed by ret
            ENDM

dtoa        MACRO  dest,source,xtra    ;; convert double to ASCII string

            IFB    <source>
            .ERR <missing operand(s) in DTOA>
            EXITM
            ENDIF

            IFNB   <xtra>
            .ERR <extra operand(s) in DTOA>
            EXITM
            ENDIF

            push   ebx                 ;; save EBX
            mov    ebx, source
            push   ebx                 ;; source parameter
            lea    ebx,dest            ;; destination address
            push   ebx                 ;; destination parameter
            call   dtoaproc            ;; call dtoaproc(source,dest)
            pop    ebx                 ;; restore EBX
            ENDM

atod        MACRO  source,xtra         ;; convert ASCII string to integer in EAX
                                       ;; offset of terminating character in ESI

            IFB    <source>
            .ERR <missing operand in ATOD>
            EXITM
            ENDIF

            IFNB   <xtra>
            .ERR <extra operand(s) in ATOD>
            EXITM
            ENDIF

            lea    eax,source          ;; source address to EAX
            push   eax                 ;; source parameter on stack
            call   atodproc            ;; call atodproc(source)
                                       ;; parameter removed by ret
            ENDM

output      MACRO  string,xtra         ;; display string

            IFB    <string>
            .ERR <missing operand in OUTPUT>
            EXITM
            ENDIF

            IFNB   <xtra>
            .ERR <extra operand(s) in OUTPUT>
            EXITM
            ENDIF

            push   eax                 ;; save EAX
            lea    eax,string          ;; string address
            push   eax                 ;; string parameter on stack
            call   outproc             ;; call outproc(string)
            pop    eax                 ;; restore EAX
            ENDM

input       MACRO  dest,length,xtra    ;; read string from keyboard

            IFB    <length>
            .ERR <missing operand(s) in INPUT>
            EXITM
            ENDIF

            IFNB   <xtra>
            .ERR <extra operand(s) in INPUT>
            EXITM
            ENDIF

            push   ebx                 ;; save EBX
            lea    ebx,dest            ;; destination address
            push   ebx                 ;; dest parameter on stack
            mov    ebx,length          ;; length of buffer
            push   ebx                 ;; length parameter on stack
            call   inproc              ;; call inproc(dest,length)
            pop    ebx                 ;; restore EBX
            ENDM


.NOLISTMACRO ; suppress macro expansion listings
.LIST        ; begin listing
