(define (problem blocks-22-1)
(:domain blocks)
(:objects
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
    (CLEAR P)
    (CLEAR O)
    (CLEAR G)
    (CLEAR A)
    (CLEAR N)
    (ONTABLE Q)
    (ONTABLE I)
    (ONTABLE K)
    (ONTABLE A)
    (ONTABLE N)
    (ON P U)
    (ON U Q)
    (ON O F)
    (ON F I)
    (ON G R)
    (ON R L)
    (ON L M)
    (ON M V)
    (ON V B)
    (ON B S)
    (ON S J)
    (ON J T)
    (ON T D)
    (ON D H)
    (ON H C)
    (ON C E)
    (ON E K)
    (HANDEMPTY)
)
(:goal (and
    (ON C T)
    (ON T L)
    (ON L I)
    (ON I E)
    (ON E G)
    (ON G B)
    (ON B J)
    (ON J V)
    (ON V K)
    (ON K H)
    (ON H A)
    (ON A S)
    (ON S D)
    (ON D M)
    (ON M R)
    (ON R Q)
    (ON Q O)
    (ON O P)
    (ON P F)
    (ON F N)
    (ON N U)
)
)
)
