(define (problem blocks-30-0)
(:domain blocks)
(:objects
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
    (CLEAR J)
    (CLEAR U)
    (CLEAR D1)
    (CLEAR E)
    (CLEAR M)
    (CLEAR N)
    (ONTABLE P)
    (ONTABLE C1)
    (ONTABLE K)
    (ONTABLE A1)
    (ONTABLE L)
    (ONTABLE O)
    (ONTABLE N)
    (ON J R)
    (ON R G)
    (ON G Q)
    (ON Q W)
    (ON W B1)
    (ON B1 T)
    (ON T C1)
    (ON U X)
    (ON X K)
    (ON D1 H)
    (ON H I)
    (ON I D)
    (ON D Z)
    (ON Z A)
    (ON A F)
    (ON F C)
    (ON C A1)
    (ON E S)
    (ON S Y)
    (ON Y B)
    (ON B V)
    (ON V L)
    (ON M O)
    (HANDEMPTY)
)
(:goal (and
    (ON C1 I)
    (ON I X)
    (ON X H)
    (ON H U)
    (ON U B1)
    (ON B1 K)
    (ON K A)
    (ON A N)
    (ON N D)
    (ON D E)
    (ON E O)
    (ON O R)
    (ON R V)
    (ON V D1)
    (ON D1 F)
    (ON F J)
    (ON J T)
    (ON T M)
    (ON M P)
    (ON P S)
    (ON S A1)
    (ON A1 Y)
    (ON Y Z)
    (ON Z C)
    (ON C W)
    (ON W G)
    (ON G Q)
    (ON Q B)
    (ON B L)
)
)
)
