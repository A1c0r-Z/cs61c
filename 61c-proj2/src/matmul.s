.globl matmul

.text
# =======================================================
# FUNCTION: Matrix Multiplication of 2 integer matrices
#   d = matmul(m0, m1)
# Arguments:
#   a0 (int*)  is the pointer to the start of m0
#   a1 (int)   is the # of rows (height) of m0
#   a2 (int)   is the # of columns (width) of m0
#   a3 (int*)  is the pointer to the start of m1
#   a4 (int)   is the # of rows (height) of m1
#   a5 (int)   is the # of columns (width) of m1
#   a6 (int*)  is the pointer to the the start of d
# Returns:
#   None (void), sets d = matmul(m0, m1)
# Exceptions:
#   Make sure to check in top to bottom order!
#   - If the dimensions of m0 do not make sense,
#     this function terminates the program with exit code 38
#   - If the dimensions of m1 do not make sense,
#     this function terminates the program with exit code 38
#   - If the dimensions of m0 and m1 don't match,
#     this function terminates the program with exit code 38
# =======================================================
matmul:
    
    
    # Error checks
    bge x0 a1 exception
    bge x0 a2 exception
    bge x0 a4 exception
    bge x0 a5 exception
    bne a2 a4 exception
    # Prologue
    addi sp sp -16
    sw ra 0(sp)
    sw s0 4(sp)
    sw s1 8(sp)
    sw s2 12(sp)

    # register init
    mv s0 x0 #for row
    mv s2 a6 #for address
    
outer_loop_start:
    bge s0 a1 outer_loop_end
    mv s1 x0
inner_loop_start:
    bge s1 a5 inner_loop_end
    
    # call dot
    addi sp sp -28
    sw a0 0(sp)
    sw a1 4(sp)
    sw a2 8(sp)
    sw a3 12(sp)
    sw a4 16(sp)
    sw a5 20(sp)
    sw a6 24(sp)
    slli t0 s1 2
    add a1 a3 t0 # pointer1
    li a3 1
    mv a4 a5
    jal dot #a0 pointer0 #a1 pointer1 #a2 num #a3 stride0 #a4 stride2 
    sw a0 0(s2)
    addi s2 s2 4
    lw a0 0(sp)
    lw a1 4(sp)
    lw a2 8(sp)
    lw a3 12(sp)
    lw a4 16(sp)
    lw a5 20(sp)
    lw a6 24(sp)
    addi sp sp 28
    
    addi s1 s1 1
    j inner_loop_start
    
inner_loop_end:
    slli t1 a2 2
    add a0 a0 t1
    addi s0 s0 1
    j outer_loop_start

outer_loop_end:
    # Epilogue
    lw ra 0(sp)
    lw s0 4(sp)
    lw s1 8(sp)
    lw s2 12(sp)
    addi sp sp 16
    jr ra
exception:
    li a0 38
    j exit
    
