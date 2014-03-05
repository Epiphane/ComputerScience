.ORIG x3000 
 
 ; Prompt for the string 
 LEA R0, PROMPT 
 PUTS 
 
 ; call the trap 
 LEA R0, STORE 
 TRAP x26 ; "get string" trap 
  ; print results 
 LEA R0, RESULT
 PUTS 
 LEA R0, STORE 
 PUTS 
 
 HALT 
 
PROMPT .STRINGZ "Enter a word: " 
RESULT .STRINGZ "\nYou entered: " 
STORE .BLKW 20 
 .END 