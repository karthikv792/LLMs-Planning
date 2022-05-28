(define (problem blocks-34-0)
(:domain blocks)
(:objects
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
    (CLEAR C1)
    (CLEAR Z)
    (ONTABLE B1)
    (ONTABLE Z)
    (ON C1 A1)
    (ON A1 H)
    (ON H M)
    (ON M P)
    (ON P D)
    (ON D T)
    (ON T J)
    (ON J B)
    (ON B G)
    (ON G I)
    (ON I F)
    (ON F K)
    (ON K H1)
    (ON H1 S)
    (ON S A)
    (ON A D1)
    (ON D1 E)
    (ON E R)
    (ON R W)
    (ON W Y)
    (ON Y N)
    (ON N C)
    (ON C V)
    (ON V L)
    (ON L G1)
    (ON G1 O)
    (ON O U)
    (ON U X)
    (ON X Q)
    (ON Q F1)
    (ON F1 E1)
    (ON E1 B1)
    (HANDEMPTY)
)
(:goal (and
    (ON P L)
    (ON L D1)
    (ON D1 F1)
    (ON F1 O)
    (ON O F)
    (ON F G1)
    (ON G1 B)
    (ON B D)
    (ON D J)
    (ON J Y)
    (ON Y B1)
    (ON B1 S)
    (ON S H)
    (ON H T)
    (ON T C1)
    (ON C1 A)
    (ON A R)
    (ON R H1)
    (ON H1 A1)
    (ON A1 X)
    (ON X M)
    (ON M E)
    (ON E Z)
    (ON Z G)
    (ON G V)
    (ON V E1)
    (ON E1 N)
    (ON N Q)
    (ON Q C)
    (ON C W)
    (ON W U)
    (ON U I)
    (ON I K)
)
)
)
