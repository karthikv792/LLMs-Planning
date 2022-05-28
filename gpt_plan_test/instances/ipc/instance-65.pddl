(define (problem blocks-32-0)
(:domain blocks)
(:objects
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
    (CLEAR V)
    (CLEAR C1)
    (CLEAR I)
    (CLEAR A)
    (CLEAR T)
    (CLEAR F1)
    (CLEAR E)
    (CLEAR K)
    (ONTABLE M)
    (ONTABLE W)
    (ONTABLE D1)
    (ONTABLE F)
    (ONTABLE T)
    (ONTABLE F1)
    (ONTABLE E)
    (ONTABLE X)
    (ON V B1)
    (ON B1 S)
    (ON S D)
    (ON D E1)
    (ON E1 C)
    (ON C O)
    (ON O L)
    (ON L Q)
    (ON Q B)
    (ON B M)
    (ON C1 G)
    (ON G Z)
    (ON Z U)
    (ON U N)
    (ON N W)
    (ON I A1)
    (ON A1 R)
    (ON R P)
    (ON P Y)
    (ON Y D1)
    (ON A J)
    (ON J H)
    (ON H F)
    (ON K X)
    (HANDEMPTY)
)
(:goal (and
    (ON M P)
    (ON P A1)
    (ON A1 N)
    (ON N S)
    (ON S U)
    (ON U W)
    (ON W Y)
    (ON Y E)
    (ON E D)
    (ON D I)
    (ON I X)
    (ON X K)
    (ON K H)
    (ON H C1)
    (ON C1 V)
    (ON V C)
    (ON C F1)
    (ON F1 R)
    (ON R A)
    (ON A L)
    (ON L G)
    (ON G E1)
    (ON E1 F)
    (ON F D1)
    (ON D1 J)
    (ON J Q)
    (ON Q B)
    (ON B Z)
    (ON Z O)
    (ON O T)
    (ON T B1)
)
)
)
