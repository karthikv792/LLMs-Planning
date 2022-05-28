(define (problem blocks-26-1)
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
    (CLEAR E)
    (CLEAR J)
    (CLEAR U)
    (CLEAR V)
    (CLEAR D)
    (CLEAR O)
    (ONTABLE P)
    (ONTABLE I)
    (ONTABLE F)
    (ONTABLE V)
    (ONTABLE K)
    (ONTABLE O)
    (ON E T)
    (ON T H)
    (ON H W)
    (ON W L)
    (ON L B)
    (ON B Z)
    (ON Z Y)
    (ON Y C)
    (ON C S)
    (ON S R)
    (ON R X)
    (ON X M)
    (ON M Q)
    (ON Q A)
    (ON A N)
    (ON N P)
    (ON J G)
    (ON G I)
    (ON U F)
    (ON D K)
    (HANDEMPTY)
)
(:goal (and
    (ON K M)
    (ON M U)
    (ON U A)
    (ON A P)
    (ON P H)
    (ON H C)
    (ON C E)
    (ON E V)
    (ON V X)
    (ON X J)
    (ON J R)
    (ON R G)
    (ON G S)
    (ON S O)
    (ON O D)
    (ON D I)
    (ON I F)
    (ON F L)
    (ON L T)
    (ON T Z)
    (ON Z W)
    (ON W N)
    (ON N Q)
    (ON Q B)
    (ON B Y)
)
)
)
