.globl relu

.text
# ==============================================================================
# FUNCTION: Performs an inplace element-wise ReLU on an array of ints
# Arguments:
#   a0 (int*) is the pointer to the array
#   a1 (int)  is the # of elements in the array
# Returns:
#   None
# Exceptions:
#   - If the length of the array is less than 1,
#     this function terminates the program with error code 36
# ==============================================================================
relu:
    # Prologue
    blt x0, a1, loop_start  # if 0 < a1 jump to loop_start
    li a0, 36               # a0 = 36
    j exit                  # return

loop_start:

    li t0, 0                # t0 = 0
    addi t1, a0, -4         # t1 = a0 - 4

loop_continue:

    bge t0, a1, loop_end    # if (t0 >= a1) break
    addi t1, t1, 4          # t1 += 4
    lw t2, 0(t1)            # t2 = arr[t1]
    addi t0, t0, 1          # t0++
    bge t2, x0, loop_continue   # if (t2 >= 0) continue
    sw x0, 0(t1)            # arr[t1] = 0
    j loop_continue         # continue

loop_end:
    
    ret
    # Epilogue


    jr ra
