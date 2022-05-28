(define (problem blocks-29-0)
(:domain blocks)
(:objects
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
    (CLEAR G)
    (CLEAR W)
    (CLEAR P)
    (CLEAR J)
    (CLEAR I)
    (CLEAR H)
    (CLEAR K)
    (ONTABLE Z)
    (ONTABLE W)
    (ONTABLE V)
    (ONTABLE B1)
    (ONTABLE N)
    (ONTABLE T)
    (ONTABLE K)
    (ON G Q)
    (ON Q A1)
    (ON A1 Z)
    (ON P C1)
    (ON C1 E)
    (ON E O)
    (ON O F)
    (ON F V)
    (ON J U)
    (ON U A)
    (ON A B1)
    (ON I R)
    (ON R S)
    (ON S C)
    (ON C M)
    (ON M L)
    (ON L B)
    (ON B X)
    (ON X Y)
    (ON Y D)
    (ON D N)
    (ON H T)
    (HANDEMPTY)
)
(:goal (and
    (ON G S)
    (ON S B1)
    (ON B1 Q)
    (ON Q U)
    (ON U F)
    (ON F Y)
    (ON Y V)
    (ON V A)
    (ON A R)
    (ON R B)
    (ON B C1)
    (ON C1 Z)
    (ON Z A1)
    (ON A1 P)
    (ON P D)
    (ON D X)
    (ON X T)
    (ON T H)
    (ON H I)
    (ON I J)
    (ON J N)
    (ON N E)
    (ON E O)
    (ON O C)
    (ON C M)
    (ON M L)
    (ON L K)
    (ON K W)
)
)
)
