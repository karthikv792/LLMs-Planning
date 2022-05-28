(define (problem blocks-30-1)
(:domain blocks)
(:objects
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
    (CLEAR I)
    (CLEAR G)
    (CLEAR U)
    (ONTABLE Q)
    (ONTABLE S)
    (ONTABLE U)
    (ON I V)
    (ON V C)
    (ON C F)
    (ON F E)
    (ON E D1)
    (ON D1 H)
    (ON H Y)
    (ON Y J)
    (ON J B)
    (ON B K)
    (ON K A)
    (ON A L)
    (ON L Q)
    (ON G X)
    (ON X P)
    (ON P Z)
    (ON Z R)
    (ON R D)
    (ON D C1)
    (ON C1 N)
    (ON N W)
    (ON W O)
    (ON O M)
    (ON M B1)
    (ON B1 A1)
    (ON A1 T)
    (ON T S)
    (HANDEMPTY)
)
(:goal (and
    (ON S Z)
    (ON Z P)
    (ON P A1)
    (ON A1 U)
    (ON U Q)
    (ON Q C1)
    (ON C1 I)
    (ON I G)
    (ON G W)
    (ON W D)
    (ON D H)
    (ON H T)
    (ON T L)
    (ON L R)
    (ON R J)
    (ON J D1)
    (ON D1 N)
    (ON N C)
    (ON C V)
    (ON V B)
    (ON B E)
    (ON E X)
    (ON X B1)
    (ON B1 F)
    (ON F Y)
    (ON Y K)
    (ON K O)
    (ON O M)
    (ON M A)
)
)
)
