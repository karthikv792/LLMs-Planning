(define (problem blocks-32-1)
(:domain blocks)
(:objects
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
    (CLEAR T)
    (CLEAR I)
    (ONTABLE A)
    (ONTABLE J)
    (ON T H)
    (ON H B)
    (ON B A)
    (ON I E)
    (ON E N)
    (ON N S)
    (ON S D1)
    (ON D1 B1)
    (ON B1 K)
    (ON K E1)
    (ON E1 Z)
    (ON Z C1)
    (ON C1 A1)
    (ON A1 W)
    (ON W X)
    (ON X F1)
    (ON F1 R)
    (ON R F)
    (ON F O)
    (ON O D)
    (ON D C)
    (ON C L)
    (ON L M)
    (ON M V)
    (ON V P)
    (ON P G)
    (ON G Q)
    (ON Q U)
    (ON U Y)
    (ON Y J)
    (HANDEMPTY)
)
(:goal (and
    (ON P D1)
    (ON D1 N)
    (ON N U)
    (ON U Z)
    (ON Z S)
    (ON S A1)
    (ON A1 T)
    (ON T E)
    (ON E G)
    (ON G R)
    (ON R Q)
    (ON Q I)
    (ON I E1)
    (ON E1 W)
    (ON W J)
    (ON J X)
    (ON X B1)
    (ON B1 A)
    (ON A V)
    (ON V F1)
    (ON F1 L)
    (ON L C1)
    (ON C1 M)
    (ON M B)
    (ON B D)
    (ON D O)
    (ON O F)
    (ON F H)
    (ON H Y)
    (ON Y K)
    (ON K C)
)
)
)
