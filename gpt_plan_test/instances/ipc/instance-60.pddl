(define (problem blocks-29-1)
(:domain blocks)
(:objects
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
    (CLEAR O)
    (CLEAR Y)
    (ONTABLE V)
    (ONTABLE Y)
    (ON O X)
    (ON X I)
    (ON I M)
    (ON M A)
    (ON A A1)
    (ON A1 R)
    (ON R T)
    (ON T C1)
    (ON C1 W)
    (ON W D)
    (ON D Q)
    (ON Q Z)
    (ON Z J)
    (ON J G)
    (ON G S)
    (ON S H)
    (ON H K)
    (ON K C)
    (ON C U)
    (ON U B1)
    (ON B1 L)
    (ON L N)
    (ON N F)
    (ON F E)
    (ON E B)
    (ON B P)
    (ON P V)
    (HANDEMPTY)
)
(:goal (and
    (ON Q B1)
    (ON B1 A1)
    (ON A1 L)
    (ON L K)
    (ON K P)
    (ON P G)
    (ON G R)
    (ON R W)
    (ON W Y)
    (ON Y B)
    (ON B O)
    (ON O V)
    (ON V S)
    (ON S U)
    (ON U E)
    (ON E I)
    (ON I F)
    (ON F A)
    (ON A T)
    (ON T X)
    (ON X M)
    (ON M C1)
    (ON C1 Z)
    (ON Z N)
    (ON N H)
    (ON H J)
    (ON J D)
    (ON D C)
)
)
)
