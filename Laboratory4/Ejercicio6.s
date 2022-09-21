@ Ejercicio3.s
@ D. Jaime
@ A simple example illustrating how one
@ can call the C function scanf() from assembly.
@ The C example for this code would be:
@     printf( "num 1? " );
@     scanf( "%d", &num1 )
@     printf( "num 2? " );
@     scanf( "%d", &num2 );
@     print( "your input: %d\n", num1 + num2 ) ;
@
@ ---------------------------------------
@     Data Section
@ ---------------------------------------
	
        .data
        .balign 4	
prompt1:.asciz  "x: "
format: .asciz  "%d"
num1:   .int    0
num2:   .int    0
output: .asciz  "The result is %d\n"
	
@ ---------------------------------------
@     Code Section
@ ---------------------------------------
	
.text
.global main
.extern printf
.extern scanf

main:   push    {ip, lr}         @ push return address + dummy register for alignment
        ldr     r0, =prompt1      @ print the prompt
        bl      printf

        ldr     r0, =format      @ call scanf, and pass address of format
        ldr     r1, =num1        @ string and address of num in r0, and r1,
        bl      scanf            @ respectively.


	Ldr     r1, =num1         @ print num formatted by output string.
        Ldr     r1, [r1]

	mov	r2, #6	
	mul	r5, r1, r1
	mul	r5, r5, r2
	mov	r2, #9
	mul	r4, r1, r2
	add	r5, r5, r4
	add	r1, r5, #2


        ldr     r0, =output
        bl      printf

        pop     {ip, pc}         @ pop return address into pc