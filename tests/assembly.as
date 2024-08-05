other: .string "abc"         
more: .data -1
x: .data 23
.extern VOID
macr my_macro
mov x, *r1
inc VOID
endmacr 

macr mcr2
THIS MACRO HAS NO MEANING


SOME NEWLINES
endmacr 
.entry x
my_macro
my_macro
LOOP: mov r1, x
dec x
jmp LOOP