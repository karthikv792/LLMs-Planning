(define (problem blocks-19-1)
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
    (CLEAR A)
    (CLEAR Q)
    (CLEAR O)
    (ONTABLE E)
    (ONTABLE D)
    (ONTABLE K)
    (ON A H)
    (ON H L)
    (ON L C)
    (ON C S)
    (ON S F)
    (ON F P)
    (ON P M)
    (ON M G)
    (ON G J)
    (ON J N)
    (ON N I)
    (ON I R)
    (ON R E)
    (ON Q D)
    (ON O B)
    (ON B K)
    (HANDEMPTY)
)
(:goal (and
    (ON L C)
    (ON C S)
    (ON S I)
    (ON I J)
    (ON J K)
    (ON K E)
    (ON E B)
    (ON B H)
    (ON H A)
    (ON A D)
    (ON D P)
    (ON P N)
    (ON N O)
    (ON O R)
    (ON R Q)
    (ON Q F)
    (ON F G)
    (ON G M)
)
)
)
