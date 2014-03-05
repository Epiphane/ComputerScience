;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;                  Lab 4, Part 4                    ;
;           Thomas Steinke & Elliot Fiske           ;
;             	   QUIZ ON DRANK                    ;
;                                                   ;
; REGISTERS USED:                                   ;
; R0: Character inputs                              ;
; R1: Current Score                                 ;
; R3: Temporary calculations                        ;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

.ORIG x3000

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;                    PROGRAM                        ;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

AND R1,R1,#0		; Init R1 (score) to 0

ASKQ1	LD R0,Q1	; Load Question
	PUTS		; Display question
	LD R0,Q1O	; Load options into memory
	PUTS		; Write options
	GETC		; Input answer
	OUT
	
	LD R3,ANTI_1
	ADD R3,R0,R3	; Subtract "1" from their answer
	LEA R0,Q1_PTS1	; Load first point value's address into memory
	ADD R0,R0,R3	; Move to the next score for each number past it
	LDR R0,R0,#0	; Load the point value of what you put in

	ADD R1,R1,R0	; Add score to current score

ASKQ2	LD R0,Q2	; Load Question
	PUTS		; Display question
	LD R0,Q2O	; Load options into memory
	PUTS		; Write options
	GETC		; Input answer
	OUT
	
	LD R3,ANTI_1
	ADD R3,R0,R3	; Subtract "1" from their answer
	LEA R0,Q2_PTS1	; Load first point value's address into memory
	ADD R0,R0,R3	; Move to the next score for each number past it
	LDR R0,R0,#0	; Load the point value of what you put in

	ADD R1,R1,R0	; Add score to current score

ASKQ3	LD R0,Q3	; Load Question
	PUTS		; Display question
	LD R0,Q3O	; Load options into memory
	PUTS		; Write options
	GETC		; Input answer
	OUT
	
	LD R3,ANTI_1
	ADD R3,R0,R3	; Subtract "1" from their answer
	LEA R0,Q3_PTS1	; Load first point value's address into memory
	ADD R0,R0,R3	; Move to the next score for each number past it
	LDR R0,R0,#0	; Load the point value of what you put in

	ADD R1,R1,R0	; Add score to current score

ENDQUIZ	ADD R0,R1,#-7	; Subtract 7 from score
	BRnz RESULT1	; If score between 0 and 7, result 1
	ADD R0,R0,#-9	; Subtract another 9
	BRnz RESULT2	; If between 7 and 16, result 2
	ADD R0,R0,#-8	; Subtract another 8
	BRnz RESULT1	; If between 17 and 25, result 3
	; otherwise result 4

RESULT4 LD R0,SITUATION4
	BRnzp KILLBILL

RESULT3 LD R0,SITUATION3
	BRnzp KILLBILL

RESULT2 LD R0,SITUATION2
	BRnzp KILLBILL

RESULT1 LD R0,SITUATION1

KILLBILL PUTS
	 HALT
	
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;                    CONSTANTS                      ;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
ANTI_1 .FILL x-31

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;                    QUESTIONS                      ;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

SITUATION1 .FILL SIT1
SITUATION2 .FILL SIT2
SITUATION3 .FILL SIT3
SITUATION4 .FILL SIT4

Q1 .FILL QUESTION1
Q2 .FILL QUESTION2
Q3 .FILL QUESTION3
Q1O .FILL Q1_OPT1
Q2O .FILL Q2_OPT1
Q3O .FILL Q3_OPT1

Q1_PTS1 .FILL #3
Q1_PTS4 .FILL #0
Q1_PTS2 .FILL #10
Q1_PTS3 .FILL #8

Q2_PTS3 .FILL #10
Q2_PTS2 .FILL #8
Q2_PTS1 .FILL #7
Q2_PTS4 .FILL #6

Q3_PTS4 .FILL #7
Q3_PTS2 .FILL #6
Q3_PTS3 .FILL #10
Q3_PTS1 .FILL #4

QUESTION1  .STRINGZ "\n\nWhich is the superior whiskey?\n"
Q1_OPT1    .STRINGZ "1.Jim Beam\n2.Diet Coke\n3.Jack Daniels\n4.Fireball Whiskey\n"
QUESTION2  .STRINGZ "\n\nWhat's the best mixed drink?\n"
Q2_OPT1    .STRINGZ "1.Whiskey Sour\n2.Long Island Iced Tea\n3.Jägerbomb\n4.White Russian\n"
QUESTION3  .STRINGZ "\n\nWhat's the best number of drinks for programming?\n"
Q3_OPT1    .STRINGZ "1.6\n2.4\n3.5\n4.3\n"

SIT1 .STRINGZ "\n\nYou're not even buzzed!\n"
SIT2 .STRINGZ "\n\nWant some water?\n"
SIT3 .STRINGZ "\n\nHey, you're a pretty shpeshul teechir\n"
SIT4 .STRINGZ "\n\nMaybe you should sober up a little before teaching...\n"

.END