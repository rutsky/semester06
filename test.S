/*
 * test.S
 * Test block that will be appended to firmware end and runned from some point.
 * Vladimir Rutsky <altsysrq@gmail.com>
 * 15.03.2009
 */

    .section    .icode,"ax",%progbits

    .align      2

        .global     testFunc
        .type       testFunc,%function
testFunc:
        add     r0, r0, r1
.end:
        .size   testFunc,.end-memset
