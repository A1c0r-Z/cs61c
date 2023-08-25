.globl read_matrix

.text
# ==============================================================================
# FUNCTION: Allocates memory and reads in a binary file as a matrix of integers
#
# FILE FORMAT:
#   The first 8 bytes are two 4 byte ints representing the # of rows and columns
#   in the matrix. Every 4 bytes afterwards is an element of the matrix in
#   row-major order.
# Arguments:
#   a0 (char*) is the pointer to string representing the filename
#   a1 (int*)  is a pointer to an integer, we will set it to the number of rows
#   a2 (int*)  is a pointer to an integer, we will set it to the number of columns
# Returns:
#   a0 (int*)  is the pointer to the matrix in memory
# Exceptions:
#   - If malloc returns an error,
#     this function terminates the program with error code 26
#   - If you receive an fopen error or eof,
#     this function terminates the program with error code 27
#   - If you receive an fclose error or eof,
#     this function terminates the program with error code 28
#   - If you receive an fread error or eof,
#     this function terminates the program with error code 29
# ==============================================================================
read_matrix:

    # Prologue
    addi sp sp -16
    sw ra 0(sp)
    sw s0 4(sp)
    sw s1 8(sp)
    sw s2 12(sp)
    # read a0 & a1
        # call fopen
        addi sp sp -12
        sw a0 0(sp)
        sw a1 4(sp)
        sw a2 8(sp)
        mv a1 x0    #read only
        jal ra fopen
        mv s1 a0
        lw a0 0(sp)
        lw a1 4(sp)
        lw a2 8(sp)
        addi sp sp 12
        addi t0 x0 -1
        beq s1 t0 fopen_error
        # s1 is the file descriptor
        
        #call fread
        addi sp sp -12
        sw a0 0(sp)
        sw a1 4(sp)
        sw a2 8(sp)
        mv a0 s1
        li a2 4
        jal ra fread #a0 file #a1 pointer for row #a2 4byte
        li a2 4
        bne a2 a0 fread_error
        lw a0 0(sp)
        lw a1 4(sp)
        lw a2 8(sp)
        addi sp sp 12
        
        addi sp sp -12
        sw a0 0(sp)
        sw a1 4(sp)
        sw a2 8(sp)
        mv a0 s1
        mv a1 a2
        li a2 4
        jal ra fread
        li a2 4
        bne a2 a0 fread_error
        lw a0 0(sp)
        lw a1 4(sp)
        lw a2 8(sp)
        addi sp sp 12
   # read matrix
        #call malloc 
        addi sp sp -4
        sw a0 0(sp)
        lw t1 0(a1)
        lw t2 0(a2)
        mul a0 t1 t2
        slli a0 a0 2
        mv s2 a0
        jal ra malloc
        mv s0 a0
        lw a0 0(sp)
        addi sp sp 4
        beq s0 x0 malloc_error
        #s0 is the pointer
        
        #call fread
        addi sp sp -12
        sw a0 0(sp)
        sw a1 4(sp)
        sw a2 8(sp)
        mv a0 s1
        mv a1 s0
        mv a2 s2 
        jal ra fread
        mv a2 s2 
        bne a2 a0 fread_error
        lw a0 0(sp)
        lw a1 4(sp)
        lw a2 8(sp)
        addi sp sp 12
        
   #close file
   mv a0 s1
   jal ra fclose
   addi t0 x0 -1
   beq t0 a0 fclose_error
        
    mv a0 s0
    # Epilogue
    lw ra 0(sp)
    lw s0 4(sp)
    lw s1 8(sp)
    lw s2 12(sp)
    addi sp sp 16
    jr ra
    
fopen_error:
    li a0 27
    j error_end
malloc_error:
    li a0 26
    j error_end
fread_error:
    addi sp sp 12 #recover stack
    li a0 29
    j error_end
fclose_error:
    li a0 28
error_end:
    lw ra 0(sp)
    lw s0 4(sp)
    lw s1 8(sp)
    lw s2 12(sp)
    addi sp sp 16
    j exit
    
    
    
    