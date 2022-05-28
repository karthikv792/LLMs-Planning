(define (problem blocks-36-0)
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
    (CLEAR H)
    (CLEAR P)
    (CLEAR I)
    (ONTABLE H1)
    (ONTABLE X)
    (ONTABLE I1)
    (ON H C1)
    (ON C1 K)
    (ON K Z)
    (ON Z R)
    (ON R G1)
    (ON G1 H1)
    (ON P E1)
    (ON E1 J1)
    (ON J1 Q)
    (ON Q L)
    (ON L C)
    (ON C N)
    (ON N D)
    (ON D M)
    (ON M F)
    (ON F F1)
    (ON F1 O)
    (ON O A)
    (ON A T)
    (ON T A1)
    (ON A1 V)
    (ON V D1)
    (ON D1 J)
    (ON J Y)
    (ON Y W)
    (ON W X)
    (ON I U)
    (ON U B)
    (ON B S)
    (ON S B1)
    (ON B1 E)
    (ON E G)
    (ON G I1)
    (HANDEMPTY)
)
(:goal (and
    (ON J O)
    (ON O J1)
    (ON J1 G1)
    (ON G1 I)
    (ON I T)
    (ON T B1)
    (ON B1 E1)
    (ON E1 D)
    (ON D X)
    (ON X N)
    (ON N M)
    (ON M A1)
    (ON A1 W)
    (ON W H)
    (ON H B)
    (ON B Z)
    (ON Z G)
    (ON G U)
    (ON U I1)
    (ON I1 F)
    (ON F P)
    (ON P H1)
    (ON H1 S)
    (ON S V)
    (ON V F1)
    (ON F1 E)
    (ON E D1)
    (ON D1 C1)
    (ON C1 Y)
    (ON Y Q)
    (ON Q K)
    (ON K R)
    (ON R A)
    (ON A C)
    (ON C L)
)
)
)
