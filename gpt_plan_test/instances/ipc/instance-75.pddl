(define (problem blocks-37-0)
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
    (CLEAR N)
    (CLEAR C1)
    (ONTABLE G1)
    (ONTABLE K)
    (ON N X)
    (ON X S)
    (ON S A)
    (ON A H1)
    (ON H1 L)
    (ON L F)
    (ON F G)
    (ON G B)
    (ON B V)
    (ON V W)
    (ON W A1)
    (ON A1 M)
    (ON M K1)
    (ON K1 O)
    (ON O E1)
    (ON E1 P)
    (ON P I1)
    (ON I1 E)
    (ON E J1)
    (ON J1 D)
    (ON D C)
    (ON C T)
    (ON T Q)
    (ON Q F1)
    (ON F1 Z)
    (ON Z B1)
    (ON B1 U)
    (ON U H)
    (ON H D1)
    (ON D1 I)
    (ON I J)
    (ON J R)
    (ON R Y)
    (ON Y G1)
    (ON C1 K)
    (HANDEMPTY)
)
(:goal (and
    (ON T K1)
    (ON K1 U)
    (ON U G)
    (ON G Q)
    (ON Q E)
    (ON E J)
    (ON J G1)
    (ON G1 D1)
    (ON D1 H)
    (ON H F)
    (ON F A1)
    (ON A1 I1)
    (ON I1 C)
    (ON C J1)
    (ON J1 B1)
    (ON B1 L)
    (ON L S)
    (ON S C1)
    (ON C1 W)
    (ON W Y)
    (ON Y E1)
    (ON E1 Z)
    (ON Z R)
    (ON R K)
    (ON K B)
    (ON B O)
    (ON O X)
    (ON X F1)
    (ON F1 H1)
    (ON H1 N)
    (ON N M)
    (ON M A)
    (ON A D)
    (ON D V)
    (ON V P)
    (ON P I)
)
)
)
