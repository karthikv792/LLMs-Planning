(define (problem blocks-36-1)
(:domain blocks)
(:objects
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
    (CLEAR J1)
    (CLEAR C1)
    (ONTABLE Z)
    (ONTABLE E1)
    (ON J1 E)
    (ON E K)
    (ON K H1)
    (ON H1 B1)
    (ON B1 J)
    (ON J G1)
    (ON G1 Q)
    (ON Q F1)
    (ON F1 M)
    (ON M H)
    (ON H U)
    (ON U F)
    (ON F R)
    (ON R C)
    (ON C Y)
    (ON Y X)
    (ON X B)
    (ON B G)
    (ON G N)
    (ON N A1)
    (ON A1 S)
    (ON S W)
    (ON W O)
    (ON O A)
    (ON A L)
    (ON L I)
    (ON I P)
    (ON P D)
    (ON D I1)
    (ON I1 T)
    (ON T V)
    (ON V Z)
    (ON C1 D1)
    (ON D1 E1)
    (HANDEMPTY)
)
(:goal (and
    (ON G R)
    (ON R N)
    (ON N I1)
    (ON I1 Y)
    (ON Y L)
    (ON L D)
    (ON D H)
    (ON H K)
    (ON K C1)
    (ON C1 T)
    (ON T D1)
    (ON D1 Z)
    (ON Z F1)
    (ON F1 E1)
    (ON E1 J)
    (ON J H1)
    (ON H1 F)
    (ON F W)
    (ON W Q)
    (ON Q I)
    (ON I B1)
    (ON B1 P)
    (ON P C)
    (ON C U)
    (ON U A1)
    (ON A1 V)
    (ON V S)
    (ON S J1)
    (ON J1 M)
    (ON M O)
    (ON O X)
    (ON X A)
    (ON A G1)
    (ON G1 E)
    (ON E B)
)
)
)
