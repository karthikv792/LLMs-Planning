(define (problem blocks-25-0)
(:domain blocks)
(:objects
    Y
    X
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
    (CLEAR A)
    (CLEAR U)
    (CLEAR X)
    (CLEAR P)
    (ONTABLE B)
    (ONTABLE F)
    (ONTABLE E)
    (ONTABLE P)
    (ON A V)
    (ON V C)
    (ON C R)
    (ON R S)
    (ON S H)
    (ON H O)
    (ON O D)
    (ON D W)
    (ON W Y)
    (ON Y G)
    (ON G J)
    (ON J Q)
    (ON Q M)
    (ON M B)
    (ON U I)
    (ON I K)
    (ON K T)
    (ON T F)
    (ON X N)
    (ON N L)
    (ON L E)
    (HANDEMPTY)
)
(:goal (and
    (ON L T)
    (ON T W)
    (ON W N)
    (ON N O)
    (ON O S)
    (ON S Y)
    (ON Y A)
    (ON A R)
    (ON R C)
    (ON C G)
    (ON G J)
    (ON J D)
    (ON D B)
    (ON B K)
    (ON K P)
    (ON P M)
    (ON M X)
    (ON X E)
    (ON E V)
    (ON V F)
    (ON F I)
    (ON I Q)
    (ON Q H)
    (ON H U)
)
)
)
