(define (problem blocks-22-0)
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
    (CLEAR N)
    (CLEAR P)
    (CLEAR V)
    (CLEAR U)
    (ONTABLE H)
    (ONTABLE P)
    (ONTABLE R)
    (ONTABLE E)
    (ON N H)
    (ON V M)
    (ON M S)
    (ON S D)
    (ON D B)
    (ON B J)
    (ON J L)
    (ON L G)
    (ON G C)
    (ON C Q)
    (ON Q R)
    (ON U I)
    (ON I A)
    (ON A T)
    (ON T K)
    (ON K O)
    (ON O F)
    (ON F E)
    (HANDEMPTY)
)
(:goal (and
    (ON H O)
    (ON O M)
    (ON M Q)
    (ON Q G)
    (ON G S)
    (ON S I)
    (ON I N)
    (ON N P)
    (ON P U)
    (ON U B)
    (ON B D)
    (ON D F)
    (ON F E)
    (ON E C)
    (ON C K)
    (ON K A)
    (ON A L)
    (ON L R)
    (ON R T)
    (ON T V)
    (ON V J)
)
)
)
