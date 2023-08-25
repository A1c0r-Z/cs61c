.globl classify

.text
# =====================================
# COMMAND LINE ARGUMENTS
# =====================================
# Args:
#   a0 (int)        argc
#   a1 (char**)     argv
#   a1[1] (char*)   pointer to the filepath string of m0
#   a1[2] (char*)   pointer to the filepath string of m1
#   a1[3] (char*)   pointer to the filepath string of input matrix
#   a1[4] (char*)   pointer to the filepath string of output file
#   a2 (int)        silent mode, if this is 1, you should not print
#                   anything. Otherwise, you should print the
#                   classification and a newline.
# Returns:
#   a0 (int)        Classification
# Exceptions:
#   - If there are an incorrect number of command line args,
#     this function terminates the program with exit code 31
#   - If malloc fails, this function terminates the program with exit code 26
#
# Usage:
#   main.s <M0_PATH> <M1_PATH> <INPUT_PATH> <OUTPUT_PATH>
classify:


    li a3, 5
    beq a0, a3 pass
    li a0 31
    j exit
pass: 
    addi sp sp -48
    sw ra 0(sp)
    sw s0 44(sp)
    sw s1 4(sp)
    sw s2 8(sp)
    sw s3 12(sp)
    sw s4 16(sp)#m0 row
    sw s5 20(sp)#m0 col
    sw s6 24(sp)#m1 row
    sw s7 28(sp)#m1 col
    sw s8 32(sp)#input row
    sw s9 40(sp)#input col
    
    
    
    
    lw s1 4(a1)
    lw s2 8(a1)
    lw s3 12(a1)
    
    
    # Read pretrained m0
    addi sp sp -12
    sw a0 0(sp)
    sw a1 4(sp)
    sw a2 8(sp)
    li a0 4
    jal ra malloc
    beq a0 x0 malloc_error
    mv s0 a0
    li a0 4
    jal ra malloc
    beq a0 x0 malloc_error
    mv a2 a0
    mv a1 s0
    mv a0 s1
    
    mv s4 a1 #ptr row
    mv s5 a2 #ptr col
    
    jal ra read_matrix
    mv s1 a0 #pointer m0
    lw a0 0(sp)
    lw a1 4(sp)
    lw a2 8(sp)
    addi sp sp 12
    # Read pretrained m1
    addi sp sp -12
    sw a0 0(sp)
    sw a1 4(sp)
    sw a2 8(sp)
    li a0 4
    jal ra malloc
    beq a0 x0 malloc_error
    mv s0 a0
    li a0 4
    jal ra malloc
    beq a0 x0 malloc_error
    mv a2 a0
    mv a1 s0
    mv a0 s2
    mv s6 a1
    mv s7 a2
    jal ra read_matrix
    mv s2 a0
    lw a0 0(sp)
    lw a1 4(sp)
    lw a2 8(sp)
    addi sp sp 12

    # Read input matrix
    addi sp sp -12
    sw a0 0(sp)
    sw a1 4(sp)
    sw a2 8(sp)
    li a0 4
    jal ra malloc
    beq a0 x0 malloc_error
    mv s0 a0
    li a0 4
    jal ra malloc
    beq a0 x0 malloc_error
    mv a2 a0
    mv a1 s0
    mv a0 s3
    mv s8 a1
    mv s9 a2
    jal ra read_matrix
    mv s3 a0
    lw a0 0(sp)
    lw a1 4(sp)
    lw a2 8(sp)
    addi sp sp 12
    ################################################
    # Compute h = matmul(m0, input)
    #s1:m0 s3:input
    addi sp sp -12
    sw a0 0(sp)
    sw a1 4(sp)
    sw a2 8(sp)
    lw t0 0(s4)
    lw t1 0(s9)
    mul a0 t0 t1
    slli a0 a0 2
    jal ra malloc
    beq a0 x0 malloc_error
    mv s0 a0  # s0 for h
    mv a6 a0  
    mv a0 s1
    
    lw t4 0(s4)
    lw t5 0(s5)
    lw t6 0(s8)
    lw t3 0(s9)
    
    mv a1 t4
    mv a2 t5
    mv a3 s3
    mv a4 t6
    mv a5 t3
    jal ra matmul
    lw a0 0(sp)
    lw a1 4(sp)
    lw a2 8(sp)
    addi sp sp 12
    
    # Compute h = relu(h)
    addi sp sp -12
    sw a0 0(sp)
    sw a1 4(sp)
    sw a2 8(sp)
    mv a0 s0
    lw t0 0(s4)
    lw t1 0(s9)
    mul a1 t0 t1
    jal ra relu
    lw a0 0(sp)
    lw a1 4(sp)
    lw a2 8(sp)
    addi sp sp 12
    
    addi sp sp -12
    sw a0 0(sp)
    sw a1 4(sp)
    sw a2 8(sp)
    mv a0 s1 #free m0
    jal ra free
    lw a0 0(sp)
    lw a1 4(sp)
    lw a2 8(sp)
    addi sp sp 12
    
    # Compute o = matmul(m1, h)
    addi sp sp -12
    sw a0 0(sp)
    sw a1 4(sp)
    sw a2 8(sp)
    lw t0 0(s6)
    lw t1 0(s9)
    mul a0 t0 t1
    slli a0 a0 2
    jal ra malloc
    beq a0 x0 malloc_error
    mv s1 a0 # change t1 to o

    lw t3 0(s6)
    lw t4 0(s7)
    lw t5 0(s4)
    lw t6 0(s9)
    
    mv a6 a0 # a6 is the pointer of o
    mv a0 s2
    mv a1 t3
    mv a2 t4
    mv a3 s0
    mv a4 t5
    mv a5 t6
    jal ra matmul
    
    lw a0 0(sp)
    lw a1 4(sp)
    lw a2 8(sp)
    addi sp sp 12
    #s1 is still the pointer of o
    
    addi sp sp -12
    sw a0 0(sp)
    sw a1 4(sp)
    sw a2 8(sp)
    mv a0 s2 #free m1
    jal ra free
    lw a0 0(sp)
    lw a1 4(sp)
    lw a2 8(sp)
    addi sp sp 12
    # Write output matrix o
    addi sp sp -12
    sw a0 0(sp)
    sw a1 4(sp)
    sw a2 8(sp)
    lw a0 16(a1)
    mv a1 s1
    
    lw t0 0(s6)
    lw t1 0(s9)
    
    mv a2 t0
    mv a3 t1
    jal ra write_matrix
    lw a0 0(sp)
    lw a1 4(sp)
    lw a2 8(sp)
    addi sp sp 12
    
    # Compute and return argmax(o)
    addi sp sp -12
    sw a0 0(sp)
    sw a1 4(sp)
    sw a2 8(sp)
    mv a0 s1
    lw t0 0(s6)
    lw t1 0(s9)
    mul a1 t0 t1
    jal ra argmax
    mv s2 a0
    lw a0 0(sp)
    lw a1 4(sp)
    lw a2 8(sp)
    addi sp sp 12
    # If enabled, print argmax(o) and newline
    bne a2 x0 end
    mv a0 s2
    jal ra print_int
    li a0 10
    jal ra print_char
end:
    mv a0 s0 # h
    jal ra free
    mv a0 s1 # o
    jal ra free
    mv a0 s3
    jal ra free
    mv a0 s4 
    jal ra free
    mv a0 s5
    jal ra free
    mv a0 s6
    jal ra free
    mv a0 s7
    jal ra free
    mv a0 s8
    jal ra free
    mv a0 s9
    jal ra free
    
    mv a0 s2
    lw ra 0(sp)
    lw s0 44(sp)
    lw s1 4(sp)
    lw s2 8(sp)
    lw s3 12(sp)
    lw s4 16(sp)#m0 row
    lw s5 20(sp)#m0 col
    lw s6 24(sp)#m1 row
    lw s7 28(sp)#m1 col
    lw s8 32(sp)#input row
    lw s9 40(sp)#input col
    addi sp sp 48
    jr ra
    
    
malloc_error:
    addi sp sp 12
    li a0 26

    lw ra 0(sp)
    lw s0 44(sp)
    lw s1 4(sp)
    lw s2 8(sp)
    lw s3 12(sp)
    lw s4 16(sp)#m0 row
    lw s5 20(sp)#m0 col
    lw s6 24(sp)#m1 row
    lw s7 28(sp)#m1 col
    lw s8 32(sp)#input row
    lw s9 40(sp)#input col
    addi sp sp 48
    j exit