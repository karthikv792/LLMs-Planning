(define (problem blocks-21-1)
(:domain blocks)
(:objects
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
    (CLEAR F)
    (CLEAR B)
    (CLEAR T)
    (ONTABLE M)
    (ONTABLE B)
    (ONTABLE T)
    (ON F L)
    (ON L D)
    (ON D I)
    (ON I R)
    (ON R A)
    (ON A J)
    (ON J H)
    (ON H S)
    (ON S Q)
    (ON Q G)
    (ON G U)
    (ON U O)
    (ON O P)
    (ON P K)
    (ON K C)
    (ON C E)
    (ON E N)
    (ON N M)
    (HANDEMPTY)
)
(:goal (and
    (ON Q L)
    (ON L J)
    (ON J C)
    (ON C M)
    (ON M N)
    (ON N F)
    (ON F U)
    (ON U E)
    (ON E D)
    (ON D T)
    (ON T S)
    (ON S K)
    (ON K H)
    (ON H B)
    (ON B I)
    (ON I R)
    (ON R O)
    (ON O P)
    (ON P A)
    (ON A G)
)
)
)
