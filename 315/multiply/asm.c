int main() {
__asm {
/* Thomas Steinke & Elliot Fiske
 * Multiplier
 */

    /* Does one section of get numbers & multiply */
    .global iocycle
iocycle:
    push {r4, r5, lr}    

cycle:
    /* Get first number */
    ldr r0, printdata
    mov r1, 1
    bl printf

    ldr r0, =scanstr
    mov r1, sp
    push {r1}
    bl __isoc99_scanf
    pop {r4}

    /* Get second number */
    ldr r0, printdata
    mov r1, r4
    bl printf

    ldr r0, =scanstr
    mov r1, sp
    push {r1}
    bl __isoc99_scanf
    pop {r5}

    /* Compute product */
    mov r0, r4
    mov r1, r5
    bl mult

    /* Move product */
    mov r1, r0
    ldr r0, printdata+4
    bl printf

    /* Again? */
    bl getchar
    cmp r0, 121
    beq cycle
endio:
    pop {r4, r5, pc}

    .global main
main:
    push {lr}

    bl iocycle

    pop {pc}
end:

    /* Add the numbers in r0 and r1 */
    .global add
add:
    push {lr}
    
    /* Get XOR */
    eors r3, r0, r1

    /* Get AND */
    ands r2, r0, r1
    
    /* Put the XOR in r0 */
    mov r0, r3

    beq endadd

    /* Shift the carry left 1 */
    lsls r1, r2, 1

    /* Add the XOR + AND(shifted) */
    bl add
endadd:
    pop {pc}


    /* Multiply the numbers in r0 and r1 */
    .global mult
mult:
    push {r4, r5, r6, lr}

    /* Product = 0 */
    mov r4, 0

    /* Pass parameters to variable registers */
    mov r5, r0
    mov r6, r1
while:
    /* If LSB(r5) == 0 Skip to endwhile */ 
    ands r3, r5, 1
    beq endwhile

    /* Add multiwhatever */
    mov r0, r4
    mov r1, r6
    bl add
    mov r4, r0

endwhile:
    /* Shift multiplier/multiplicand */
    lsrs r5, r5, 1
    lsls r6, r6, 1

    /* Break on r5 == 0 */
    cmp r5, 0
    bne while

endmult:
    mov r0, r4
    pop {r4, r5, r6, pc}

scanstr:
    .asciz " %d"

printdata:
    .word enternum
    .word product

enternum:
    .asciz "Enter number %d: "
    
product:
    .asciz "Product is: %d\nAgain? "
}
}
