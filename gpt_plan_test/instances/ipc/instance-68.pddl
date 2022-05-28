(define (problem blocks-33-1)
(:domain blocks)
(:objects
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
    (CLEAR D1)
    (CLEAR L)
    (CLEAR E1)
    (CLEAR U)
    (CLEAR Z)
    (ONTABLE Y)
    (ONTABLE D1)
    (ONTABLE A1)
    (ONTABLE R)
    (ONTABLE N)
    (ONTABLE Z)
    (ON D M)
    (ON M W)
    (ON W F)
    (ON F F1)
    (ON F1 X)
    (ON X I)
    (ON I Q)
    (ON Q C1)
    (ON C1 K)
    (ON K J)
    (ON J O)
    (ON O V)
    (ON V B1)
    (ON B1 C)
    (ON C B)
    (ON B S)
    (ON S G)
    (ON G G1)
    (ON G1 T)
    (ON T P)
    (ON P H)
    (ON H Y)
    (ON L E)
    (ON E A)
    (ON A A1)
    (ON E1 R)
    (ON U N)
    (HANDEMPTY)
)
(:goal (and
    (ON T N)
    (ON N C)
    (ON C G1)
    (ON G1 V)
    (ON V A)
    (ON A I)
    (ON I Y)
    (ON Y B1)
    (ON B1 F)
    (ON F B)
    (ON B H)
    (ON H J)
    (ON J Z)
    (ON Z U)
    (ON U P)
    (ON P E1)
    (ON E1 E)
    (ON E G)
    (ON G Q)
    (ON Q D)
    (ON D L)
    (ON L F1)
    (ON F1 A1)
    (ON A1 M)
    (ON M S)
    (ON S W)
    (ON W O)
    (ON O D1)
    (ON D1 C1)
    (ON C1 X)
    (ON X K)
    (ON K R)
)
)
)
