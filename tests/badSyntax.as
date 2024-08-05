;this file will intend to have one error
;of each type that realtes to syntax
;these are comments so wont be raise as error
this will

;now some bas assembly code
Z: .entry x
x: .data 1, 2, 3
x: .string "acb"

.extern EXT
.entry EXT

mov EXT, #-1, Z
dec #-1
.extern x
LOOP:

.data "not a number"
.data 1 2 3 4 5
.string -1

bne VOID