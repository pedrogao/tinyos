; $ nasm -f bin hello.asm -o hello.bin
; $ qemu-system-x86_64 -fda hello.bin
bits 16            ; 16位
org 0x7c00         ; 0x7c00开始执行

boot:              ; boot 启动 
    mov si, hello  ; si = hello
    mov ah, 0x0e   ; ah = 0x0e

.loop:             ; loop
    lodsb
    or al, al
    jz halt
    int 0x10       ; 中断
    jmp .loop      ; jump loop
halt:              ; halt
    cli            ; close interrupt
    hlt            ; halt

hello: db "Hello, World!", 0

times 510 - ($ - $$) db 0 ; padding
dw 0xaa55                 ; define words, 2 bytes