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
prompt1:.asciz  "num1 "
prompt2:.asciz  "num2 "
prompt3:.asciz  "operation (1:+,2:-,3:*,4:/) "
format:.asciz  "%d"
num1:   .int    0
num2:   .int    0
operand:.int    0
output0: .asciz  "Error: Invalid operand\n"
output1: .asciz  "The result is %d + %d = %d\n"
output2: .asciz  "The result is %d - %d = %d\n"
output3: .asciz  "The result is %d * %d = %d\n"
output4: .asciz  "The result is %d / %d = %d\n"
	
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

	ldr     r0, =prompt3      @ print the prompt
        bl      printf

        ldr     r0, =format      @ call scanf, and pass address of format
        ldr     r1, =operand        @ string and address of num in r0, and r1,
        bl      scanf            @ respectively.

        ldr     r0, =prompt2      @ print the prompt
        bl      printf

        ldr     r0, =format      @ call scanf, and pass address of format
        ldr     r1, =num2        @ string and address of num in r0, and r1,
        bl      scanf            @ respectively.

	Ldr     r3, =operand         @ print num formatted by output string.
        Ldr     r3, [r3]	

	Ldr     r1, =num1         @ print num formatted by output string.
        Ldr     r1, [r1]

	Ldr     r2, =num2         @ print num formatted by output string.
        Ldr     r2, [r2]

	cmp	r3, #1
	beq	Suma

	cmp	r3, #2
	beq	Resta

	cmp	r3, #3
	beq	Multi

	cmp	r3, #4
	beq	Divi
	
	ldr     r0, =output0
        bl      printf
	b	Final
	

Suma:	add	r3, r1, r2
	ldr     r0, =output1
        bl      printf
	b	Final

Resta:	sub	r3, r1, r2
	ldr     r0, =output2
        bl      printf
	b	Final

Multi:	mul	r3, r1, r2
	ldr     r0, =output3
        bl      printf
	b	Final

Divi:	mov	r3, #0
	mov	r4, r1
Divi2:	add	r3, r3, #1
	subs	r4, r4, r2
	bpl	Divi2
	sub	r3, r3, #1
	ldr     r0, =output4
        bl      printf
	b	Final

Final:  pop     {ip, pc}         @ pop return address into pc