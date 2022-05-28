(define (problem blocks-40-0)
(:domain blocks)
(:objects
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
    (CLEAR D)
    (CLEAR O)
    (CLEAR C)
    (CLEAR T)
    (CLEAR M)
    (CLEAR H)
    (ONTABLE D)
    (ONTABLE W)
    (ONTABLE N)
    (ONTABLE T)
    (ONTABLE M)
    (ONTABLE H)
    (ON O J1)
    (ON J1 H1)
    (ON H1 A)
    (ON A M1)
    (ON M1 J)
    (ON J G1)
    (ON G1 I1)
    (ON I1 Q)
    (ON Q B)
    (ON B P)
    (ON P L)
    (ON L V)
    (ON V F1)
    (ON F1 Y)
    (ON Y K1)
    (ON K1 N1)
    (ON N1 D1)
    (ON D1 Z)
    (ON Z X)
    (ON X S)
    (ON S E)
    (ON E C1)
    (ON C1 R)
    (ON R U)
    (ON U K)
    (ON K E1)
    (ON E1 B1)
    (ON B1 G)
    (ON G W)
    (ON C A1)
    (ON A1 F)
    (ON F I)
    (ON I L1)
    (ON L1 N)
    (HANDEMPTY)
)
(:goal (and
    (ON A B1)
    (ON B1 S)
    (ON S I1)
    (ON I1 A1)
    (ON A1 Y)
    (ON Y R)
    (ON R K)
    (ON K J1)
    (ON J1 D)
    (ON D N)
    (ON N F)
    (ON F G)
    (ON G E)
    (ON E N1)
    (ON N1 T)
    (ON T U)
    (ON U C)
    (ON C L1)
    (ON L1 D1)
    (ON D1 P)
    (ON P V)
    (ON V Q)
    (ON Q M1)
    (ON M1 H)
    (ON H K1)
    (ON K1 E1)
    (ON E1 W)
    (ON W F1)
    (ON F1 O)
    (ON O J)
    (ON J Z)
    (ON Z C1)
    (ON C1 M)
    (ON M X)
    (ON X L)
    (ON L H1)
    (ON H1 B)
    (ON B I)
    (ON I G1)
)
)
)
