;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;                 CSC 225, Lab 5                    ;
;                 Thomas Steinke                    ;
;                 **************                    ;
;                    Trap x26                       ;
;                                                   ;
; Used Registers:                                   ;
; R1: Stores addresses for ST and JMP               ;
; R7: (Precondition) passed with PC for UP1         ;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

.ORIG x3300

STI R7,SAVE_PC1		; Store the PC from UP1 in x32FF

LD R1,INTERRUPT		; Set R1 to 0100 0000 0000 0000 (interrupt enable)
STI R1,KBSR		; Store interrupt enable in KBSR

LD R1,UP2		; Load address of UP2...
JMP R1			; And enable hyperdrive

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; CONSTANTS                                         ;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
SAVE_PC1 .FILL x32FF
KBSR .FILL xFE00
INTERRUPT .FILL x4000
UP2 .FILL x3400

.END