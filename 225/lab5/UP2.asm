;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;                 CSC 225, Lab 5                    ;
;                 Thomas Steinke                    ;
;                 **************                    ;
;                 User Program 2                    ;
;                                                   ;
; Used Registers:                                   ;
; R0: Character storage/output                      ;
;     Counter to display *                          ;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

.ORIG x3400

START_COUNTER 	AND R0,R0,#0 		; Reset R0 to 0

INC_COUNT	ADD R0,R0,#1		; Add 1 to R0
		BRzp INC_COUNT		; If it's not negative, repeat

DISP_STAR	LD R0,ASTERISK		; Load * into R0
		OUT			; Display to screen
		BRnzp START_COUNTER	; Rinse and repeat

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; CONSTANTS                                         ;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
ASTERISK .FILL x2A

.END