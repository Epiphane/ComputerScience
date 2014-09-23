@ Thomas Steinke & Elliot Fiske
@ Multiplier (myadd.s file)

    .arch armv6
    .fpu vfp
    .text
    
    /* Add the numbers in r0 and r1 */
    .global add
add:
    push {lr}

keepadding:
    /* Get AND */
    ands r2, r0, r1

    /* Get XOR */
    eor r0, r0, r1

    /* Uses the result of 'and' to break or not */
    beq endadd

    /* Shift the carry left 1 */
    lsls r1, r2, #1

    /* Add the XOR + AND(shifted) */
    bl keepadding

endadd:
    pop {pc}
