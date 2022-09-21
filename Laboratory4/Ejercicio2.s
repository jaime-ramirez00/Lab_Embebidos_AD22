@  Ejercicio2.s
@  D. Jaime
@  Illustrates how to pass 2 ints by value
@  to a function that adds them up and returns	
@  the sum in r0.
@ ---------------------------------------
@    Data Section
@ ---------------------------------------
	
        .data
        .balign 4	
string1: .asciz "\nAddress = %X, Value = %X\n, yAddress = %X\n"
a:      .word   0xAAAAAAAA
b:      .word   0xBBBBBBBB
c:      .word   0xCCCCCCCC
y:	.word   0

	
@ ---------------------------------------
@    Code Section
@ ---------------------------------------
	
        .text
        .global main
        .extern printf
	
@ ---------------------------------------
@ sumFunc: gets 2 ints in r1 and r2, adds
@ them up and saves the results in
@ r0.	
storeA:
        push    {ip, lr}
	ldr	r3, =y
        str	r2, [r3]
	ldr     r0, =string1
	bl	printf
        pop     {ip, pc}

storeB:
        push    {ip, lr}
	ldr	r3, =y
	ldr	r2, [r1] 
        str	r2, [r3] 
	ldr     r0, =string1
	bl	printf
        pop     {ip, pc}

@ ---------------------------------------
@ main: passes 2 ints to sumFunc and prints
@ the resulting value using printf	
main:   push    {ip, lr}         @ push return address + dummy register for alignment
        ldr     r1, =a           @ get address of a into r1
        ldr     r2, [r1]         @ get a into r1
	bl      storeA

	
        ldr     r1, =b           @ get address of b into r2
        bl      storeB	 	 @ pass (r1, r2) to sumFunc 
@ gets sum back in r0
	ldr	r3, =y
        ldr     r1, =c		 @ get address of c into r2
	ldr	r2, [r1] 
        str     r2, [r3]         @ store r0 into c
	ldr     r0, =string1
	bl	printf


@ return to OS	
        pop     {ip, pc}	@ pop return address into pc
