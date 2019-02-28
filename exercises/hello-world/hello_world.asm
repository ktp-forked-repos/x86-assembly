section .data
str: db '', 0

section .text
global hello
hello:
    mov eax, str
    ret
