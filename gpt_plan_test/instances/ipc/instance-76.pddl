(define (problem blocks-37-1)
(:domain blocks)
(:objects
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
    (CLEAR S)
    (CLEAR Q)
    (CLEAR G1)
    (CLEAR I)
    (CLEAR C1)
    (ONTABLE B1)
    (ONTABLE N)
    (ONTABLE G1)
    (ONTABLE L)
    (ONTABLE C1)
    (ON S E)
    (ON E F)
    (ON F W)
    (ON W V)
    (ON V X)
    (ON X E1)
    (ON E1 G)
    (ON G I1)
    (ON I1 M)
    (ON M P)
    (ON P D1)
    (ON D1 H1)
    (ON H1 B1)
    (ON Q T)
    (ON T J)
    (ON J A1)
    (ON A1 A)
    (ON A Y)
    (ON Y R)
    (ON R J1)
    (ON J1 K1)
    (ON K1 O)
    (ON O H)
    (ON H F1)
    (ON F1 Z)
    (ON Z D)
    (ON D B)
    (ON B U)
    (ON U K)
    (ON K C)
    (ON C N)
    (ON I L)
    (HANDEMPTY)
)
(:goal (and
    (ON F Z)
    (ON Z O)
    (ON O J1)
    (ON J1 Y)
    (ON Y H1)
    (ON H1 M)
    (ON M G1)
    (ON G1 K)
    (ON K E)
    (ON E I1)
    (ON I1 G)
    (ON G J)
    (ON J I)
    (ON I X)
    (ON X V)
    (ON V A1)
    (ON A1 E1)
    (ON E1 R)
    (ON R Q)
    (ON Q A)
    (ON A F1)
    (ON F1 T)
    (ON T S)
    (ON S N)
    (ON N C)
    (ON C P)
    (ON P D)
    (ON D D1)
    (ON D1 C1)
    (ON C1 B1)
    (ON B1 H)
    (ON H K1)
    (ON K1 B)
    (ON B L)
    (ON L U)
    (ON U W)
)
)
)
