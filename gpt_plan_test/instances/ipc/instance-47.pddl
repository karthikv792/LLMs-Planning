(define (problem blocks-23-0)
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
    (CLEAR V)
    (CLEAR Q)
    (CLEAR B)
    (CLEAR U)
    (CLEAR F)
    (ONTABLE G)
    (ONTABLE Q)
    (ONTABLE B)
    (ONTABLE W)
    (ONTABLE S)
    (ON V A)
    (ON A K)
    (ON K N)
    (ON N D)
    (ON D I)
    (ON I M)
    (ON M J)
    (ON J O)
    (ON O T)
    (ON T L)
    (ON L C)
    (ON C P)
    (ON P E)
    (ON E G)
    (ON U R)
    (ON R W)
    (ON F H)
    (ON H S)
    (HANDEMPTY)
)
(:goal (and
    (ON E K)
    (ON K I)
    (ON I J)
    (ON J A)
    (ON A Q)
    (ON Q F)
    (ON F N)
    (ON N D)
    (ON D W)
    (ON W C)
    (ON C H)
    (ON H P)
    (ON P R)
    (ON R T)
    (ON T S)
    (ON S V)
    (ON V B)
    (ON B U)
    (ON U L)
    (ON L O)
    (ON O G)
    (ON G M)
)
)
)
