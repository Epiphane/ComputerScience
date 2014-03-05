;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;                 CSC 225, Lab 5                    ;
;                 Thomas Steinke                    ;
;                 **************                    ;
;            Interrupt Service Routine              ;
;                                                   ;
; Used Registers:                                   ;
; R0: Character storage/output                      ;
; R1: Set to x0000 for clearing KBDR                ;
;     Loads PC from UP1 to store in stack           ;
; R6: Stack pointer                                 ;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

.ORIG x3500

LOAD_CHAR	LDI R0,KBDR	; Load character pressed into R0

EMPTY_KBDR	AND R1,R1,#0	; Clear R1
		STI R1,KBSR	; Empty KBSR

		LDI R1,SAVE_PC1	; Load PC from UP1 into R1
		STR R1,R6,#0	; Replace PC from UP2 with PC from UP1

RTI

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; CONSTANTS                                         ;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
SAVE_PC1 .FILL x32FF
KBSR .FILL xFE00
KBDR .FILL xFE02

.END