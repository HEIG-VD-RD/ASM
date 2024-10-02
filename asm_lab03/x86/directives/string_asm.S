.include "./string_asm.inc"
.global to_upper_case
.extern copy_string
.equiv MAX_STRING_SIZE, 30 


.data
tmp_string:
	.space MAX_STRING_SIZE
	
.text
to_upper_case:
	# Sauvegarder la valeur de ebp sur la pile
	pushl %ebp
	# Initialiser ebp à la valeur courante de esp
	movl %esp, %ebp
	# Sauvegarder la valeur de ebx, esi et edi sur la pile
	pushl %ebx
	pushl %esi
	pushl %edi
	# Déplacer la valeur du premier argument de la pile dans edi
	movl 8(%ebp), %edi
	# Initialiser esi à 0
	xorl %esi, %esi

loop:
	# Déplacer le byte pointé par edi+esi dans edx
	movzbl (%edi,%esi), %edx

	cmpl $ascii_a, %edx
	jl copy
	cmpl $ascii_z, %edx
	jg copy
	# Soustraire la constante to_upper_sub de la valeur edx pour obtenir la majuscule
	subl $to_upper_sub, %edx

copy:
    # Déplacer l'adresse de tmp_string+esi dans ebx
	leal tmp_string(%esi), %ebx
	# Copier la valeur de dl dans l'adresse pointée par ebx
	movb %dl, (%ebx)
	# Incrémenter esi de 1
	incl %esi
	#Test si le caractère est nulle
	testb %dl, %dl
	je end_string
	cmpl $MAX_STRING_SIZE, %esi
	jl loop
	# Déplacer l'adresse de tmp_string+MAX_STRING_SIZE-1 dans ebx
	leal tmp_string+MAX_STRING_SIZE-1, %ebx
	movb $0, (%ebx)

end_string:
    # Déplacer l'adresse de tmp_string dans ecx
	leal tmp_string, %ecx
	movl %esi, %edx
	# Empiler les valeurs de edx, ecx, edi et esi sur la pile
	pushl %edx
	pushl %ecx
	pushl %edi

	call copy_string
	# Désempiler
	addl $16, %esp

	movl %edx, %eax

	# Return
	popl %edi
	popl %esi
	popl %ebx
	movl %ebp, %esp
	popl %ebp
	ret
