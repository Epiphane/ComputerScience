@ Thomas Steinke & Elliot Fiske
@ Multiplier (mymult.s file)

    .arch armv6
    .fpu vfp
    .text
    
    /* Multiply the numbers in r0 and r1 */
    .global mult
mult:
    push {r4, r5, r6, lr, r0, r1}

    /* Pass parameters to variable registers */
    pop {r5, r6}

super:
    /* Check to see if it's easier to do rev(r5) */
    mov r4, #0
    mvn r0, r5
    cmp r0, r5

    /* If not(r5) > r5 (unsigned), then we don't want it */
    bhi prep

    /* Otherwise switch them! */
    mov r1, #1
    bl add
    mov r5, r0
    mov r4, #1

prep:
    /* Product = 0 */
    mov r0, #0

while:
    /* If LSB(r5) == 0 Skip to endwhile */ 
    ands r3, r5, #1
    beq endwhile

    /* Add multiwhatever */
    mov r1, r6
    bl add

endwhile:
    /* Shift multiplier/multiplicand */
    lsrs r5, r5, #1
    lsls r6, r6, #1

    /* Break on r5 == 0 */
    cmp r5, #0
    bne while

endmult:
    /* Don't do this if we didn't flip any bits */
    cmp r4, #0
    beq return

    mvns r0, r0
    mov r1, #1
    bl add

return:
    pop {r4, r5, r6, pc}
