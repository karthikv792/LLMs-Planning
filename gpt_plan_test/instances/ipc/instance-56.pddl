(define (problem blocks-27-1)
(:domain blocks)
(:objects
    A1
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
    (CLEAR N)
    (CLEAR H)
    (ONTABLE P)
    (ONTABLE K)
    (ON N Y)
    (ON Y O)
    (ON O E)
    (ON E U)
    (ON U P)
    (ON H C)
    (ON C F)
    (ON F G)
    (ON G Z)
    (ON Z I)
    (ON I D)
    (ON D W)
    (ON W J)
    (ON J A1)
    (ON A1 M)
    (ON M X)
    (ON X A)
    (ON A R)
    (ON R V)
    (ON V S)
    (ON S L)
    (ON L Q)
    (ON Q B)
    (ON B T)
    (ON T K)
    (HANDEMPTY)
)
(:goal (and
    (ON L D)
    (ON D B)
    (ON B Y)
    (ON Y A)
    (ON A E)
    (ON E H)
    (ON H O)
    (ON O W)
    (ON W J)
    (ON J S)
    (ON S F)
    (ON F Z)
    (ON Z G)
    (ON G A1)
    (ON A1 V)
    (ON V U)
    (ON U X)
    (ON X K)
    (ON K I)
    (ON I N)
    (ON N Q)
    (ON Q C)
    (ON C M)
    (ON M T)
    (ON T P)
    (ON P R)
)
)
)
