L0: addi $t1, $zero, 5 ; t1(2) = 5
    addi $t2, $zero, 4 ; t2(3) = 4
    add $t3, $t1, $t2 ; t3(4) = 9
L1: j L2 ; PC = 5
    sub $t4, $t1, $t2   
L2: beq $t1, $t2, L3
    bneq $t1, $t2, L4 ; PC = 9
L3: sw $t3, 2($t1)
    lw $t0, 3($t2)
L4: sw $t3, 3($t2) ; Store 9 in 7
    subi $t3, $t3, 3 ; t3(4) = 6 
    or $t0, $t1, $t3 ; t0(1) = 7
    andi $t4, $t2, 6 ; t4(5) = 4 
    subi $t2, $t2, 1 ; t2(3) = 3
    lw $t4, 2($t1) ; t4(5) = 9
    sll $t1, $t1, 1 ; t1(2) = 10
    srl $t2, $t2, 1 ; t2(3) = 1
