.PHONY: all clean

all:
	@nasm -f bin hello.asm -o hello.bin
	@nasm -f bin ioloop.asm -o ioloop.bin
	@nasm -f bin editor.asm -o editor.bin

clean:
	@rm *.bin