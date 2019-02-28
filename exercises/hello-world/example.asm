section .data
str: db 'Hello, World!', 0

section .text
global hello
hello:
    mov eax, str
    ret
