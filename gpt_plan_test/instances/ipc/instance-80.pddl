(define (problem blocks-39-1)
(:domain blocks)
(:objects
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
    (CLEAR C)
    (CLEAR Q)
    (CLEAR N)
    (CLEAR L1)
    (ONTABLE G1)
    (ONTABLE W)
    (ONTABLE D)
    (ONTABLE J)
    (ON C M)
    (ON M E)
    (ON E H)
    (ON H G)
    (ON G B1)
    (ON B1 G1)
    (ON Q I)
    (ON I X)
    (ON X V)
    (ON V T)
    (ON T C1)
    (ON C1 D1)
    (ON D1 B)
    (ON B A1)
    (ON A1 K1)
    (ON K1 A)
    (ON A U)
    (ON U F)
    (ON F J1)
    (ON J1 F1)
    (ON F1 R)
    (ON R Y)
    (ON Y S)
    (ON S Z)
    (ON Z E1)
    (ON E1 M1)
    (ON M1 I1)
    (ON I1 H1)
    (ON H1 K)
    (ON K W)
    (ON N L)
    (ON L P)
    (ON P D)
    (ON L1 O)
    (ON O J)
    (HANDEMPTY)
)
(:goal (and
    (ON C1 U)
    (ON U K)
    (ON K Z)
    (ON Z K1)
    (ON K1 M1)
    (ON M1 H1)
    (ON H1 X)
    (ON X G1)
    (ON G1 H)
    (ON H W)
    (ON W G)
    (ON G Q)
    (ON Q J)
    (ON J L1)
    (ON L1 B1)
    (ON B1 V)
    (ON V D)
    (ON D I1)
    (ON I1 M)
    (ON M D1)
    (ON D1 J1)
    (ON J1 I)
    (ON I E1)
    (ON E1 Y)
    (ON Y A)
    (ON A B)
    (ON B N)
    (ON N C)
    (ON C P)
    (ON P R)
    (ON R S)
    (ON S O)
    (ON O T)
    (ON T F)
    (ON F A1)
    (ON A1 E)
    (ON E L)
    (ON L F1)
)
)
)
