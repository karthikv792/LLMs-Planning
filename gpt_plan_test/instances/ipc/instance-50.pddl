(define (problem blocks-24-1)
(:domain blocks)
(:objects
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
    (CLEAR K)
    (CLEAR I)
    (ONTABLE C)
    (ONTABLE O)
    (ON K F)
    (ON F T)
    (ON T B)
    (ON B G)
    (ON G R)
    (ON R M)
    (ON M E)
    (ON E J)
    (ON J V)
    (ON V N)
    (ON N U)
    (ON U H)
    (ON H C)
    (ON I A)
    (ON A P)
    (ON P Q)
    (ON Q D)
    (ON D W)
    (ON W X)
    (ON X S)
    (ON S L)
    (ON L O)
    (HANDEMPTY)
)
(:goal (and
    (ON L C)
    (ON C P)
    (ON P Q)
    (ON Q M)
    (ON M B)
    (ON B G)
    (ON G F)
    (ON F K)
    (ON K E)
    (ON E R)
    (ON R A)
    (ON A W)
    (ON W T)
    (ON T N)
    (ON N J)
    (ON J U)
    (ON U S)
    (ON S D)
    (ON D H)
    (ON H V)
    (ON V O)
    (ON O I)
    (ON I X)
)
)
)
