(define (problem blocks-21-0)
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
    (CLEAR K)
    (CLEAR H)
    (ONTABLE F)
    (ONTABLE H)
    (ON K I)
    (ON I P)
    (ON P M)
    (ON M R)
    (ON R C)
    (ON C S)
    (ON S E)
    (ON E Q)
    (ON Q N)
    (ON N B)
    (ON B G)
    (ON G J)
    (ON J A)
    (ON A U)
    (ON U D)
    (ON D O)
    (ON O T)
    (ON T L)
    (ON L F)
    (HANDEMPTY)
)
(:goal (and
    (ON H L)
    (ON L B)
    (ON B E)
    (ON E C)
    (ON C S)
    (ON S P)
    (ON P O)
    (ON O R)
    (ON R K)
    (ON K T)
    (ON T U)
    (ON U Q)
    (ON Q J)
    (ON J N)
    (ON N G)
    (ON G I)
    (ON I D)
    (ON D A)
    (ON A M)
    (ON M F)
)
)
)
