	.text			    # Define beginning of text section
	.global	_start		# Define entry _start

_start:
	li x5, 0
	li x6, 5
loop:
	addi x5, x5, 1
	bgeu x6, x5, loop     # if x6 >= x5, jmp loop

stop:
	j stop			    # Infinite loop to stop execution

	.end			    # End of file
