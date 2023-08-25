.globl argmax

.text
# =================================================================
# FUNCTION: Given a int array, return the index of the largest
#   element. If there are multiple, return the one
#   with the smallest index.
# Arguments:
#   a0 (int*) is the pointer to the start of the array
#   a1 (int)  is the # of elements in the array
# Returns:
#   a0 (int)  is the first index of the largest element
# Exceptions:
#   - If the length of the array is less than 1,
#     this function terminates the program with error code 36
# =================================================================
argmax:
    # Prologue
    blt x0, a1, loop_start  # if 0 < a1 jump to loop_start
    li a0, 36               # a0 = 36
    j exit                  # return

loop_start:

    li t0, -1               # t0 = -1
    addi t1, a0, -4         # t1 = a0 - 4
    li t3, 0                # t3 = 0 (index)
    lw t4, 0(a0)            # t4 = arr[0] (value)

loop_continue:

    addi t0, t0, 1          # t0++
    addi t1, t1, 4          # t1 += 4
    bge t0, a1, loop_end    # if (t0 >= a1) break
    lw t2, 0(t1)            # t2 = arr[t1]
    blt t2, t4, loop_continue   # if (t2 < t4) continue
    mv t3, t0               # t3 = t0
    mv t4, t2               # t4 = t2
    j loop_continue         # continue

loop_end:
    # Epilogue
    mv a0, t3               # a0 = t3
    jr ra
