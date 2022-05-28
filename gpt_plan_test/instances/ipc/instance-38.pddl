(define (problem blocks-18-1)
(:domain blocks)
(:objects
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
    (CLEAR J)
    (CLEAR K)
    (ONTABLE C)
    (ONTABLE A)
    (ONTABLE K)
    (ON G P)
    (ON P N)
    (ON N E)
    (ON E R)
    (ON R I)
    (ON I F)
    (ON F O)
    (ON O L)
    (ON L B)
    (ON B D)
    (ON D Q)
    (ON Q M)
    (ON M C)
    (ON J H)
    (ON H A)
    (HANDEMPTY)
)
(:goal (and
    (ON H G)
    (ON G M)
    (ON M D)
    (ON D B)
    (ON B N)
    (ON N E)
    (ON E F)
    (ON F R)
    (ON R Q)
    (ON Q O)
    (ON O K)
    (ON K J)
    (ON J I)
    (ON I P)
    (ON P L)
    (ON L C)
    (ON C A)
)
)
)
