;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;                RECURSIVE CHAR COUNT PROGRAM                    ;
;                     By Thomas Steinke                          ;
;                                                                ;
; Registers used:                                                ;
; R0 - Character we're comparing                                 ;
; R1 - Character we're searching for                             ;
; R5 - Frame Pointer                                             ;
; R6 - Stack Pointer                                             ;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

.ORIG x3300

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; PREPARATION
	; Set aside space for return value
	ADD R6,R6,#-1
	; Store R7 in the stack for safekeeping
	ADD R6,R6,#-1
	STR R7,R6,#0
	; Store the frame pointer as well
	ADD R6,R6,#-1
	STR R5,R6,#0

	; Set new frame pointer
	ADD R5,R6,#-1
	; Set result to 0, and move stack pointer to top
	AND R0,R0,#0 ; R0 = 0
	STR R0,R5,#0 ; Store result

	ADD R6,R5,#0 ; R6 = R5

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; COMPUTE DAT NUMBAH
	; Load current character into R0
	LDR R0,R5,#4
	LDR R0,R0,#0

	; If it's \0 (null pointer), return 0
	BRz KILL_ME

	; Load character we're looking for into R1
	LDR R1,R5,#5

	; Compare the two
	; First you have to negate the character...
	NOT R0,R0
	ADD R0,R0,#1
	; ...Then you compare
	ADD R2,R0,R1

	; Not a match? Don't increment result
	BRnp SKIP
		; Otherwise, congratz!
		LDR R0,R5,#0 ; Load result into R0
		ADD R0,R0,#1 ; Add 1 to result
		STR R0,R5,#0 ; And store it again

SKIP ; Business as usual...
	; Load comparing character into R0...
	LDR R0,R5,#5
	; ...and store it in the new stack
	ADD R6,R6,#-1
	STR R0,R6,#0
	
	; Load character pointer into R0 again...
	LDR R0,R5,#4
	; Then add 1 to it (next character)
	ADD R0,R0,#1
	; ...and store it in the new stack
	ADD R6,R6,#-1
	STR R0,R6,#0
	
	; Recursion!
	LD R0,CHAR_COUNT
	JSRR R0

	; Load returned value into R0
	LDR R0,R6,#0
	LDR R1,R5,#0
	ADD R0,R1,R0 ; R0 is now the return value

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; WE DONE
KILL_ME
	; Put the result into the return value
	; LDR R0,R5,#0 <- R0 was already just set
	STR R0,R5,#3

	; Move R6 back to the useful data
	ADD R6,R5,#1

	; Return R6 and R5 back to their original states
	LDR R5,R6,#0
	ADD R6,R6,#1

	; Grab R7 again
	LDR R7,R6,#0
	
	; Point R6 at the return value
	ADD R6,R6,#1
	
	; We have to go back
	RET

CHAR_COUNT .FILL x3300

.END
