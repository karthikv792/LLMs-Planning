(define (problem blocks-41-1)
(:domain blocks)
(:objects
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
    (CLEAR P)
    (CLEAR L1)
    (CLEAR B)
    (CLEAR W)
    (CLEAR G1)
    (CLEAR N)
    (ONTABLE H)
    (ONTABLE K1)
    (ONTABLE B)
    (ONTABLE R)
    (ONTABLE G)
    (ONTABLE N)
    (ON P C1)
    (ON C1 J)
    (ON J E1)
    (ON E1 X)
    (ON X A)
    (ON A C)
    (ON C O)
    (ON O J1)
    (ON J1 V)
    (ON V H1)
    (ON H1 O1)
    (ON O1 H)
    (ON L1 S)
    (ON S M1)
    (ON M1 U)
    (ON U I1)
    (ON I1 T)
    (ON T D)
    (ON D A1)
    (ON A1 D1)
    (ON D1 K)
    (ON K Q)
    (ON Q Y)
    (ON Y F1)
    (ON F1 E)
    (ON E Z)
    (ON Z M)
    (ON M B1)
    (ON B1 N1)
    (ON N1 K1)
    (ON W F)
    (ON F I)
    (ON I R)
    (ON G1 L)
    (ON L G)
    (HANDEMPTY)
)
(:goal (and
    (ON J1 K1)
    (ON K1 V)
    (ON V J)
    (ON J H1)
    (ON H1 A1)
    (ON A1 P)
    (ON P C1)
    (ON C1 Y)
    (ON Y K)
    (ON K S)
    (ON S N1)
    (ON N1 H)
    (ON H D)
    (ON D G)
    (ON G I1)
    (ON I1 O)
    (ON O N)
    (ON N X)
    (ON X O1)
    (ON O1 F1)
    (ON F1 F)
    (ON F Z)
    (ON Z E)
    (ON E G1)
    (ON G1 L)
    (ON L W)
    (ON W L1)
    (ON L1 Q)
    (ON Q E1)
    (ON E1 B1)
    (ON B1 R)
    (ON R T)
    (ON T M)
    (ON M U)
    (ON U M1)
    (ON M1 D1)
    (ON D1 B)
    (ON B C)
    (ON C I)
    (ON I A)
)
)
)
