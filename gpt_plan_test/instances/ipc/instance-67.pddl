(define (problem blocks-33-0)
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
    (CLEAR K)
    (CLEAR U)
    (CLEAR Y)
    (CLEAR A1)
    (ONTABLE E1)
    (ONTABLE R)
    (ONTABLE C1)
    (ONTABLE I)
    (ON K F)
    (ON F F1)
    (ON F1 M)
    (ON M P)
    (ON P B1)
    (ON B1 D)
    (ON D A)
    (ON A Z)
    (ON Z G)
    (ON G O)
    (ON O H)
    (ON H W)
    (ON W B)
    (ON B T)
    (ON T E1)
    (ON U L)
    (ON L R)
    (ON Y C)
    (ON C D1)
    (ON D1 S)
    (ON S G1)
    (ON G1 V)
    (ON V J)
    (ON J X)
    (ON X Q)
    (ON Q N)
    (ON N E)
    (ON E C1)
    (ON A1 I)
    (HANDEMPTY)
)
(:goal (and
    (ON V C)
    (ON C B)
    (ON B G1)
    (ON G1 D1)
    (ON D1 K)
    (ON K F)
    (ON F P)
    (ON P D)
    (ON D Q)
    (ON Q B1)
    (ON B1 U)
    (ON U E)
    (ON E X)
    (ON X F1)
    (ON F1 A1)
    (ON A1 G)
    (ON G T)
    (ON T Y)
    (ON Y R)
    (ON R Z)
    (ON Z O)
    (ON O E1)
    (ON E1 M)
    (ON M J)
    (ON J A)
    (ON A S)
    (ON S N)
    (ON N W)
    (ON W H)
    (ON H C1)
    (ON C1 L)
    (ON L I)
)
)
)
