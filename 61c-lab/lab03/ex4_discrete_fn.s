.globl f # this allows other files to find the function f

# f takes in two arguments:
# a0 is the value we want to evaluate f at
# a1 is the address of the "output" array (defined above).
# The return value should be stored in a0
f:
    # Your code here
    mv t0 a0
    addi t0 t0 3
    mv t1 a1
    slli t2, t0, 2 # step 1 (see above)
    add t2, t2, t1 # step 2 (see above)
    lw t3, 0(t2)   # step 3 (see above)
    mv a0 t3
#     addi sp sp -4
#     sw ra, 0(sp)
#     mv t0 a0
#     addi t0 t0 3
#     slli t0 t0 2
#     add t0 t0 a1
#     lw a0 0(t0)
#     lw ra 0(sp)
#     addi sp sp 4
    # This is how you return from a function. You'll learn more about this later.
    # This should be the last line in your program.
    jr ra
