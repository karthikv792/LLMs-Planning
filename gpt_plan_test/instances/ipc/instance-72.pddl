(define (problem blocks-35-1)
(:domain blocks)
(:objects
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
    (CLEAR M)
    (CLEAR G1)
    (ONTABLE L)
    (ONTABLE T)
    (ONTABLE D1)
    (ON C Q)
    (ON Q G)
    (ON G B1)
    (ON B1 W)
    (ON W X)
    (ON X U)
    (ON U Y)
    (ON Y R)
    (ON R D)
    (ON D H1)
    (ON H1 B)
    (ON B A)
    (ON A N)
    (ON N J)
    (ON J E)
    (ON E V)
    (ON V H)
    (ON H P)
    (ON P F)
    (ON F C1)
    (ON C1 S)
    (ON S K)
    (ON K O)
    (ON O Z)
    (ON Z F1)
    (ON F1 A1)
    (ON A1 I)
    (ON I I1)
    (ON I1 L)
    (ON M E1)
    (ON E1 T)
    (ON G1 D1)
    (HANDEMPTY)
)
(:goal (and
    (ON I F1)
    (ON F1 C)
    (ON C R)
    (ON R J)
    (ON J F)
    (ON F S)
    (ON S Z)
    (ON Z G1)
    (ON G1 D)
    (ON D V)
    (ON V C1)
    (ON C1 H1)
    (ON H1 H)
    (ON H M)
    (ON M B1)
    (ON B1 I1)
    (ON I1 L)
    (ON L A)
    (ON A A1)
    (ON A1 B)
    (ON B Y)
    (ON Y K)
    (ON K E)
    (ON E P)
    (ON P D1)
    (ON D1 U)
    (ON U Q)
    (ON Q X)
    (ON X G)
    (ON G E1)
    (ON E1 N)
    (ON N T)
    (ON T W)
    (ON W O)
)
)
)
