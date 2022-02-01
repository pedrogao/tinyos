	.text			    # Define beginning of text section
	.global	_start		# Define entry _start

_start:
	li x5, 0
	li x6, 5
2:
	addi x5, x5, 1
	bgeu x6, x5, 2b     # if x6 >= x5, pc = 2b

stop:
	j stop			    # Infinite loop to stop execution

	.end			    # End of file
