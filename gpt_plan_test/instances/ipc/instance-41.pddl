(define (problem blocks-20-0)
(:domain blocks)
(:objects
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
    (CLEAR S)
    (CLEAR N)
    (CLEAR P)
    (ONTABLE H)
    (ONTABLE N)
    (ONTABLE K)
    (ON S C)
    (ON C M)
    (ON M Q)
    (ON Q B)
    (ON B T)
    (ON T J)
    (ON J L)
    (ON L E)
    (ON E I)
    (ON I O)
    (ON O G)
    (ON G F)
    (ON F A)
    (ON A D)
    (ON D H)
    (ON P R)
    (ON R K)
    (HANDEMPTY)
)
(:goal (and
    (ON K E)
    (ON E N)
    (ON N R)
    (ON R D)
    (ON D G)
    (ON G H)
    (ON H O)
    (ON O A)
    (ON A L)
    (ON L J)
    (ON J F)
    (ON F M)
    (ON M I)
    (ON I Q)
    (ON Q B)
    (ON B P)
    (ON P T)
    (ON T S)
    (ON S C)
)
)
)
