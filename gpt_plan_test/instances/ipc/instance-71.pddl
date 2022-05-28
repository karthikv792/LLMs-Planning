(define (problem blocks-35-0)
(:domain blocks)
(:objects
    I1
    H1
    G1
    F1
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
    (CLEAR A1)
    (CLEAR F)
    (CLEAR X)
    (CLEAR I1)
    (CLEAR H1)
    (ONTABLE D)
    (ONTABLE J)
    (ONTABLE W)
    (ONTABLE B1)
    (ONTABLE G1)
    (ON A1 Q)
    (ON Q O)
    (ON O Z)
    (ON Z D)
    (ON F S)
    (ON S B)
    (ON B I)
    (ON I R)
    (ON R J)
    (ON X U)
    (ON U V)
    (ON V C)
    (ON C C1)
    (ON C1 K)
    (ON K E1)
    (ON E1 H)
    (ON H F1)
    (ON F1 G)
    (ON G P)
    (ON P L)
    (ON L N)
    (ON N A)
    (ON A T)
    (ON T M)
    (ON M D1)
    (ON D1 W)
    (ON I1 Y)
    (ON Y E)
    (ON E B1)
    (ON H1 G1)
    (HANDEMPTY)
)
(:goal (and
    (ON C1 H)
    (ON H F1)
    (ON F1 P)
    (ON P O)
    (ON O N)
    (ON N S)
    (ON S L)
    (ON L Q)
    (ON Q R)
    (ON R B)
    (ON B Z)
    (ON Z X)
    (ON X G1)
    (ON G1 H1)
    (ON H1 M)
    (ON M D1)
    (ON D1 E)
    (ON E G)
    (ON G C)
    (ON C F)
    (ON F E1)
    (ON E1 A)
    (ON A I1)
    (ON I1 A1)
    (ON A1 I)
    (ON I Y)
    (ON Y V)
    (ON V J)
    (ON J U)
    (ON U W)
    (ON W B1)
    (ON B1 T)
    (ON T D)
    (ON D K)
)
)
)
