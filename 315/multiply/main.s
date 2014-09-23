@ Thomas Steinke & Elliot Fiske
@ Multiplier

    .arch armv6
    .fpu vfp
    .text

    /* Does one section of get numbers & multiply */
    .global iocycle
iocycle:
    push {lr}    

    /* Push sp down 16 */
    mov r0, sp
    mov r1, #-16
    bl add
    mov sp, r0

cycle:
    /* Get first number */
    ldr r0, printdata
    mov r1, #1
    bl printf

    /* Set r1 = sp + 8 */
    mov r0, sp
    mov r1, #8
    bl add
    mov r1, r0
    /* Scan in first number */
    ldr r0, =scanstrnum
    bl __isoc99_scanf

    /* Get second number */
    ldr r0, printdata
    mov r1, #2
    bl printf

    /* Set r1 = sp */
    mov r1, sp
    /* Scan in second number */
    ldr r0, =scanstrnum
    bl __isoc99_scanf
    
    /* Compute product */
    ldr r0, [sp]
    ldr r1, [sp, #8]
    bl mult

    /* Move product */
    mov r1, r0
    ldr r0, printdata+4
    bl printf

    /* Gotta get the last character out of the buffer */
    bl getchar
    bl getchar
    cmp r0, #121
    beq cycle
endio:
    
    /* Put sp back where it was */
    mov r0, sp
    mov r1, #16
    bl add
    mov sp, r0

    pop {pc}

    .global main
main:
    push {lr}

    bl iocycle

    pop {pc}
end:

scanstrnum:
    .asciz " %d"

scanstrchar:
    .asciz " %c"

printdata:
    .word enternum
    .word product

enternum:
    .asciz "Enter number %d: "
    
product:
    .asciz "Product is: %d\nAgain? "
