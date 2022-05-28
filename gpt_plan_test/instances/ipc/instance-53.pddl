(define (problem blocks-26-0)
(:domain blocks)
(:objects
    Z
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
    (CLEAR B)
    (CLEAR U)
    (CLEAR X)
    (CLEAR F)
    (CLEAR Y)
    (ONTABLE D)
    (ONTABLE R)
    (ONTABLE J)
    (ONTABLE P)
    (ONTABLE Y)
    (ON B E)
    (ON E A)
    (ON A L)
    (ON L Q)
    (ON Q I)
    (ON I Z)
    (ON Z K)
    (ON K N)
    (ON N S)
    (ON S C)
    (ON C D)
    (ON U O)
    (ON O G)
    (ON G W)
    (ON W R)
    (ON X V)
    (ON V H)
    (ON H M)
    (ON M T)
    (ON T J)
    (ON F P)
    (HANDEMPTY)
)
(:goal (and
    (ON X E)
    (ON E N)
    (ON N O)
    (ON O U)
    (ON U I)
    (ON I Z)
    (ON Z K)
    (ON K Y)
    (ON Y V)
    (ON V G)
    (ON G B)
    (ON B J)
    (ON J S)
    (ON S C)
    (ON C R)
    (ON R L)
    (ON L H)
    (ON H T)
    (ON T D)
    (ON D F)
    (ON F M)
    (ON M W)
    (ON W Q)
    (ON Q P)
    (ON P A)
)
)
)
