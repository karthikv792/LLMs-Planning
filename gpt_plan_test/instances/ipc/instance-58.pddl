(define (problem blocks-28-1)
(:domain blocks)
(:objects
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
    (CLEAR U)
    (CLEAR T)
    (CLEAR W)
    (ONTABLE B1)
    (ONTABLE O)
    (ONTABLE L)
    (ON U A)
    (ON A F)
    (ON F Y)
    (ON Y S)
    (ON S D)
    (ON D I)
    (ON I G)
    (ON G J)
    (ON J A1)
    (ON A1 K)
    (ON K V)
    (ON V R)
    (ON R B1)
    (ON T O)
    (ON W N)
    (ON N Z)
    (ON Z B)
    (ON B E)
    (ON E Q)
    (ON Q X)
    (ON X C)
    (ON C M)
    (ON M H)
    (ON H P)
    (ON P L)
    (HANDEMPTY)
)
(:goal (and
    (ON A X)
    (ON X F)
    (ON F E)
    (ON E M)
    (ON M P)
    (ON P H)
    (ON H G)
    (ON G B1)
    (ON B1 U)
    (ON U A1)
    (ON A1 O)
    (ON O B)
    (ON B R)
    (ON R L)
    (ON L T)
    (ON T W)
    (ON W Z)
    (ON Z I)
    (ON I D)
    (ON D N)
    (ON N S)
    (ON S V)
    (ON V Y)
    (ON Y Q)
    (ON Q C)
    (ON C J)
    (ON J K)
)
)
)
