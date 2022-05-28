(define (problem blocks-31-1)
(:domain blocks)
(:objects
    E1
    D1
    C1
    B1
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
    (CLEAR P)
    (CLEAR R)
    (CLEAR Z)
    (CLEAR K)
    (CLEAR N)
    (ONTABLE H)
    (ONTABLE F)
    (ONTABLE I)
    (ONTABLE U)
    (ONTABLE S)
    (ON P C)
    (ON C A)
    (ON A V)
    (ON V E)
    (ON E A1)
    (ON A1 O)
    (ON O G)
    (ON G D)
    (ON D B)
    (ON B M)
    (ON M L)
    (ON L Y)
    (ON Y D1)
    (ON D1 C1)
    (ON C1 E1)
    (ON E1 X)
    (ON X Q)
    (ON Q B1)
    (ON B1 H)
    (ON R F)
    (ON Z W)
    (ON W T)
    (ON T I)
    (ON K U)
    (ON N J)
    (ON J S)
    (HANDEMPTY)
)
(:goal (and
    (ON J G)
    (ON G C1)
    (ON C1 N)
    (ON N F)
    (ON F R)
    (ON R A1)
    (ON A1 E1)
    (ON E1 C)
    (ON C T)
    (ON T M)
    (ON M Q)
    (ON Q E)
    (ON E D)
    (ON D B1)
    (ON B1 X)
    (ON X W)
    (ON W P)
    (ON P K)
    (ON K D1)
    (ON D1 Z)
    (ON Z B)
    (ON B U)
    (ON U Y)
    (ON Y I)
    (ON I O)
    (ON O V)
    (ON V A)
    (ON A H)
    (ON H S)
    (ON S L)
)
)
)
