(define (problem blocks-28-0)
(:domain blocks)
(:objects
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
    (CLEAR X)
    (CLEAR K)
    (CLEAR T)
    (CLEAR F)
    (ONTABLE L)
    (ONTABLE Y)
    (ONTABLE U)
    (ONTABLE B1)
    (ON X G)
    (ON G J)
    (ON J V)
    (ON V A)
    (ON A N)
    (ON N A1)
    (ON A1 B)
    (ON B C)
    (ON C R)
    (ON R L)
    (ON K Y)
    (ON T O)
    (ON O U)
    (ON F Q)
    (ON Q I)
    (ON I P)
    (ON P D)
    (ON D H)
    (ON H W)
    (ON W Z)
    (ON Z S)
    (ON S E)
    (ON E M)
    (ON M B1)
    (HANDEMPTY)
)
(:goal (and
    (ON B1 N)
    (ON N A)
    (ON A Y)
    (ON Y D)
    (ON D A1)
    (ON A1 T)
    (ON T R)
    (ON R X)
    (ON X G)
    (ON G B)
    (ON B F)
    (ON F C)
    (ON C U)
    (ON U K)
    (ON K M)
    (ON M W)
    (ON W Z)
    (ON Z J)
    (ON J P)
    (ON P O)
    (ON O L)
    (ON L S)
    (ON S E)
    (ON E I)
    (ON I V)
    (ON V Q)
    (ON Q H)
)
)
)
