(define (problem blocks-43-1)
(:domain blocks)
(:objects
    Q1
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
    (CLEAR R)
    (CLEAR Q)
    (ONTABLE S)
    (ONTABLE G1)
    (ON R W)
    (ON W P1)
    (ON P1 M1)
    (ON M1 L1)
    (ON L1 T)
    (ON T O)
    (ON O P)
    (ON P C1)
    (ON C1 B1)
    (ON B1 U)
    (ON U G)
    (ON G N1)
    (ON N1 O1)
    (ON O1 F1)
    (ON F1 V)
    (ON V Y)
    (ON Y Q1)
    (ON Q1 I1)
    (ON I1 C)
    (ON C K1)
    (ON K1 H)
    (ON H F)
    (ON F H1)
    (ON H1 J)
    (ON J A)
    (ON A D1)
    (ON D1 I)
    (ON I E1)
    (ON E1 J1)
    (ON J1 Z)
    (ON Z D)
    (ON D X)
    (ON X E)
    (ON E L)
    (ON L M)
    (ON M A1)
    (ON A1 N)
    (ON N B)
    (ON B S)
    (ON Q K)
    (ON K G1)
    (HANDEMPTY)
)
(:goal (and
    (ON L N1)
    (ON N1 W)
    (ON W G1)
    (ON G1 O1)
    (ON O1 E1)
    (ON E1 U)
    (ON U I1)
    (ON I1 Q1)
    (ON Q1 X)
    (ON X V)
    (ON V Y)
    (ON Y A1)
    (ON A1 L1)
    (ON L1 J1)
    (ON J1 C)
    (ON C H1)
    (ON H1 M)
    (ON M H)
    (ON H K1)
    (ON K1 K)
    (ON K O)
    (ON O F1)
    (ON F1 Q)
    (ON Q P)
    (ON P B1)
    (ON B1 M1)
    (ON M1 B)
    (ON B P1)
    (ON P1 S)
    (ON S A)
    (ON A C1)
    (ON C1 Z)
    (ON Z E)
    (ON E F)
    (ON F N)
    (ON N D1)
    (ON D1 G)
    (ON G R)
    (ON R D)
    (ON D I)
    (ON I T)
    (ON T J)
)
)
)
