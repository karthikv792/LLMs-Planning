(define (problem blocks-23-1)
(:domain blocks)
(:objects
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
    (CLEAR M)
    (CLEAR U)
    (CLEAR F)
    (CLEAR Q)
    (CLEAR N)
    (ONTABLE B)
    (ONTABLE O)
    (ONTABLE J)
    (ONTABLE Q)
    (ONTABLE N)
    (ON M A)
    (ON A R)
    (ON R B)
    (ON U G)
    (ON G T)
    (ON T S)
    (ON S V)
    (ON V H)
    (ON H C)
    (ON C K)
    (ON K P)
    (ON P L)
    (ON L W)
    (ON W O)
    (ON F E)
    (ON E D)
    (ON D I)
    (ON I J)
    (HANDEMPTY)
)
(:goal (and
    (ON A M)
    (ON M T)
    (ON T N)
    (ON N J)
    (ON J I)
    (ON I G)
    (ON G O)
    (ON O V)
    (ON V U)
    (ON U H)
    (ON H C)
    (ON C W)
    (ON W Q)
    (ON Q E)
    (ON E K)
    (ON K R)
    (ON R P)
    (ON P B)
    (ON B S)
    (ON S F)
    (ON F L)
    (ON L D)
)
)
)
