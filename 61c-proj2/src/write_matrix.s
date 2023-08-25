.globl write_matrix

.text
# ==============================================================================
# FUNCTION: Writes a matrix of integers into a binary file
# FILE FORMAT:
#   The first 8 bytes of the file will be two 4 byte ints representing the
#   numbers of rows and columns respectively. Every 4 bytes thereafter is an
#   element of the matrix in row-major order.
# Arguments:
#   a0 (char*) is the pointer to string representing the filename
#   a1 (int*)  is the pointer to the start of the matrix in memory
#   a2 (int)   is the number of rows in the matrix
#   a3 (int)   is the number of columns in the matrix
# Returns:
#   None
# Exceptions:
#   - If you receive an fopen error or eof,
#     this function terminates the program with error code 27
#   - If you receive an fclose error or eof,
#     this function terminates the program with error code 28
#   - If you receive an fwrite error or eof,
#     this function terminates the program with error code 30
# ==============================================================================
write_matrix:

    # Prologue
    addi sp sp -12
    sw ra 0(sp)
    sw s0 4(sp)
    sw s1 8(sp)
    #
        #fopen
        addi sp sp -16
        sw a0 0(sp)
        sw a1 4(sp)
        sw a2 8(sp)
        sw a3 12(sp)
        
        addi a1 x0 1
        jal ra fopen
        mv s0 a0
    
        lw a0 0(sp)
        lw a1 4(sp)
        lw a2 8(sp)
        lw a3 12(sp)
        addi sp sp 16
        addi t0 x0 -1
        beq s0 t0 fopen_error
        #s0 is the file descriptor
        
        #write row & col
        addi sp sp -16
        sw a0 0(sp)
        sw a1 4(sp)
        sw a2 8(sp)
        sw a3 12(sp)
        mv a0 s0
        addi a1 sp 8
        li a2 2
        li a3 4
        jal ra fwrite
        li t0 2
        bne t0 a0 fwrite_error
        lw a0 0(sp)
        lw a1 4(sp)
        lw a2 8(sp)
        lw a3 12(sp)
        addi sp sp 16
        
        #write matrix
        addi sp sp -16
        sw a0 0(sp)
        sw a1 4(sp)
        sw a2 8(sp)
        sw a3 12(sp)
        mv a0 s0
        mul s1 a2 a3
        mul a2 a2 a3
        li a3 4
        jal ra fwrite
        bne s1 a0 fwrite_error
        lw a0 0(sp)
        lw a1 4(sp)
        lw a2 8(sp)
        lw a3 12(sp)
        addi sp sp 16
        
       #close file
       mv a0 s0
       jal ra fclose
       addi t0 x0 -1
       beq t0 a0 fclose_error
       
    # Epilogue
    lw ra 0(sp)
    lw s0 4(sp)
    lw s1 8(sp)
    addi sp sp 12

    jr ra
    
fopen_error:
    li a0 27
    j error_end
fwrite_error:
    addi sp sp 16
    li a0 30
    j error_end
fclose_error:
    li a0 28
error_end:
    lw ra 0(sp)
    lw s0 4(sp)
    lw s1 8(sp)
    addi sp sp 12
    j exit





