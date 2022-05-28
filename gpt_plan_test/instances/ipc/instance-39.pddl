(define (problem blocks-19-0)
(:domain blocks)
(:objects
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
    (CLEAR L)
    (CLEAR S)
    (ONTABLE B)
    (ONTABLE L)
    (ONTABLE S)
    (ON N J)
    (ON J K)
    (ON K G)
    (ON G A)
    (ON A R)
    (ON R I)
    (ON I P)
    (ON P M)
    (ON M Q)
    (ON Q C)
    (ON C D)
    (ON D H)
    (ON H O)
    (ON O E)
    (ON E F)
    (ON F B)
    (HANDEMPTY)
)
(:goal (and
    (ON H Q)
    (ON Q P)
    (ON P A)
    (ON A L)
    (ON L R)
    (ON R I)
    (ON I D)
    (ON D J)
    (ON J B)
    (ON B N)
    (ON N F)
    (ON F S)
    (ON S M)
    (ON M O)
    (ON O E)
    (ON E C)
    (ON C K)
    (ON K G)
)
)
)
