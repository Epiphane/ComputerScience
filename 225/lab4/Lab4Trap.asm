;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;                  Lab 4, Part 4                    ;
;           Thomas Steinke & Elliot Fiske           ;
;           TRAP x26 Code -- Input String           ;
;                                                   ;
; INPUT:                                            ;
; R0: Address to store the string into              ;
;                                                   ;
; OUTPUT:                                           ;
; String starting at R0: Inputted String            ;
;                                                   ;
; REGISTERS USED:                                   ;
; R0: Character inputs                              ;
; R1: Stores current address in string              ;
; R2: Temporarily stores status registers           ;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

.ORIG x3300

ST R0, STRING_ADDR		; Store value of R0 so we can retrieve it again
ST R1, SAVE_R1			; Store value of R1 so we can retrieve it again
ST R2, SAVE_R2			; Store value of R2 so we can retrieve it again
ADD R1,R0,#0			; Begin Counter

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; READ IN CHARACTER
READ_ST	LDI R0,KBSR		; Load status of keyboard
	BRzp READ_ST		; If it is not ready, repeat
	LDI R0,KBDR		; Load value of keyboard character to R0

	STR R0, R1, #0		; Put the character just entered into memory
	ADD R1, R1, #1		; Increment address of the string location

	ADD R2, R0, #-10	; If the last key was an enter...
	BRnp WRITE		; ...this will be 0, and the loop will exit.

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; ENTER PRESSED. END TRAP
END_ME	AND R0,R0,#0
	STR R0, R1, #-1		; Put a null character where the last key was

	LD R0, STRING_ADDR	; Return R0 to original value
	LD R1, SAVE_R1 		; Return R1 to original value
	LD R2, SAVE_R2 		; Return R1 to original value
	RET

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; PRINT OUT CHARACTER
WRITE	LDI R2,DSR		; Load status of display register into R2
	BRzp WRITE		; Keep checking until it is ready
	STI R0,DDR
	BRnzp READ_ST		; Read next character

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; CONSTANTS                                         ;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; String Address and R1 for retrieval
STRING_ADDR .FILL x0000
SAVE_R1 .FILL x0000
SAVE_R2 .FILL x0000

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Keyboard and display register values
KBSR .FILL xFE00
KBDR .FILL xFE02
DSR .FILL xFE04
DDR .FILL xFE06

.END

