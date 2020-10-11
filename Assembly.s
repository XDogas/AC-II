.equ SFR_BASE_HI, 0xBF88
.equ TRISB,0x6040
.equ PORTB,0x6050
.equ LATB,0x6060
.equ TRISE,0x6100
.equ PORTE,0x6110
.equ LATE,0x6120

.data
.text
.globl main

main:
	#RE0 como saida

	lui $t1,SFR_BASE_HI
	lw $t2,TRISE($t1)
	andi $t2,$t2,0xfffe
	sw $t2,TRISE($t1)

	#RB0 como entrada
	lui $t1,SFR_BASE_HI 			#so e preciso declarar 1 vez
	lw $t2,TRISB($t1)
	ori $t2,$t2,0x0001
	sw $t2,TRISB($t1)


loop:
	#RE0 = RB0
	
	lw $t1,PORTB($t0)  			
	andi $t1,$t1,0x0001
	lw $t2,LATE($t0)	
	andi $t2,$t2,0xfffe	
	or $t1,$t1,$t2		
	sw $t1,LATE($t0)	
	
	#RE0 = RBO\
	
	lw $t1,PORTB($t0) 				#so e preciso declarar 1 vez			
	andi $t1,$t1,0x0001												
	xor $t1,$t1,0x0001 											
	lw $t2,LATE($t0)											 	
	andi $t2,$t2,0xfffe 											
	or $t1,$t1,$t2   													
	sw $t1,LATE($t0)
													
	j loop
	jr $ra
	
	

	
	
	
	
	
	
	
	
