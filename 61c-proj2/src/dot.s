.globl dot

.text
# =======================================================
# FUNCTION: Dot product of 2 int arrays
# Arguments:
#   a0 (int*) is the pointer to the start of arr0
#   a1 (int*) is the pointer to the start of arr1
#   a2 (int)  is the number of elements to use
#   a3 (int)  is the stride of arr0
#   a4 (int)  is the stride of arr1
# Returns:
#   a0 (int)  is the dot product of arr0 and arr1
# Exceptions:
#   - If the number of elements to use is less than 1,
#     this function terminates the program with error code 36
#   - If the stride of either array is less than 1,
#     this function terminates the program with error code 37
# =======================================================
dot:

    # Prologue
    li t0, 1                # t0 = 1
    blt a2, t0, error1      # if (a2 < 1) goto error1
    blt a3, t0, error2      # if (a3 < 1) goto error2
    blt a4, t0, error2      # if (a4 < 1) goto error2

loop_start:

    li t0, 0                # t0 = 0 (i)

    mv t2, a0               # t2 = a0 (iaddr)
    mv t3, a1               # t3 = a1 (jaddr)
    li t4, 0                # t4 = 0 (ans)
    slli a5, a3, 2          # a5 = a3 * sizeof(int)
    slli a6, a4, 2          # a6 = a4 * sizeof(int)

loop_continue:

    lw t5, 0(t2)            # t5 = arr[t2]
    lw t6, 0(t3)            # t6 = arr[t3]
    mul t5, t5, t6          # t5 = t5 * t6
    add t4, t4, t5          # t4 += t5

    addi t0, t0, 1          # t0++
    bge t0, a2, loop_end    # if (t0 >= a2) break
    
    add t2, t2, a5          # t2 += a5
    add t3, t3, a6          # t3 += a6
    j loop_continue

loop_end:

    mv a0, t4
    # Epilogue

    jr ra

error1:

    li a0, 36
    j exit

error2:

    li a0, 37
    j exit