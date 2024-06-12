    addi $t1, $zero, 9
    beq $t1, $t2, L1
L0: addi $t2, $zero, 5
    addi $t3, $zero, 1
    add $t3, $t1, $t2
    ad      d $t1, $t3, $t1
    beq $t1, $t2, L0
    sw $t3, 10($t2)
    lw $t1, 0($t2)
L1: add $t1, $t1, $zero
    j L0
    sw $t1, 10($sp)
    lw $t1, 10($sp)