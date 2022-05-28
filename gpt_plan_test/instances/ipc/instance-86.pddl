(define (problem blocks-42-1)
(:domain blocks)
(:objects
    P1
    O1
    N1
    M1
    L1
    K1
    J1
    I1
    H1
    G1
    F1
    E1
    D1
    C1
    B1
    A1
    Z
    Y
    X
    W
    V
    U
    T
    S
    R
    Q
    P
    O
    N
    M
    L
    K
    J
    I
    H
    G
    F
    E
    D
    C
    A
    B
 - block)
(:init
    (CLEAR N1)
    (ONTABLE D1)
    (ON N1 K1)
    (ON K1 M1)
    (ON M1 X)
    (ON X Z)
    (ON Z L)
    (ON L L1)
    (ON L1 I1)
    (ON I1 J1)
    (ON J1 E1)
    (ON E1 F)
    (ON F G1)
    (ON G1 V)
    (ON V O)
    (ON O O1)
    (ON O1 P1)
    (ON P1 H1)
    (ON H1 C)
    (ON C M)
    (ON M J)
    (ON J Y)
    (ON Y P)
    (ON P A1)
    (ON A1 F1)
    (ON F1 A)
    (ON A K)
    (ON K D)
    (ON D Q)
    (ON Q S)
    (ON S E)
    (ON E W)
    (ON W B1)
    (ON B1 B)
    (ON B C1)
    (ON C1 I)
    (ON I T)
    (ON T G)
    (ON G R)
    (ON R H)
    (ON H N)
    (ON N U)
    (ON U D1)
    (HANDEMPTY)
)
(:goal (and
    (ON U Q)
    (ON Q A)
    (ON A K1)
    (ON K1 M1)
    (ON M1 G)
    (ON G P1)
    (ON P1 M)
    (ON M W)
    (ON W G1)
    (ON G1 J1)
    (ON J1 E1)
    (ON E1 Y)
    (ON Y L)
    (ON L N1)
    (ON N1 N)
    (ON N J)
    (ON J I)
    (ON I L1)
    (ON L1 E)
    (ON E C)
    (ON C B1)
    (ON B1 D)
    (ON D H)
    (ON H H1)
    (ON H1 F1)
    (ON F1 P)
    (ON P O)
    (ON O D1)
    (ON D1 I1)
    (ON I1 K)
    (ON K Z)
    (ON Z T)
    (ON T S)
    (ON S C1)
    (ON C1 A1)
    (ON A1 V)
    (ON V F)
    (ON F R)
    (ON R B)
    (ON B O1)
    (ON O1 X)
)
)
)
