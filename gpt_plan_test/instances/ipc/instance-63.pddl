(define (problem blocks-31-0)
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
    (CLEAR Y)
    (CLEAR C1)
    (CLEAR J)
    (ONTABLE M)
    (ONTABLE A)
    (ONTABLE P)
    (ON Y A1)
    (ON A1 H)
    (ON H D)
    (ON D F)
    (ON F R)
    (ON R W)
    (ON W T)
    (ON T Q)
    (ON Q G)
    (ON G L)
    (ON L V)
    (ON V O)
    (ON O N)
    (ON N D1)
    (ON D1 E1)
    (ON E1 B)
    (ON B B1)
    (ON B1 M)
    (ON C1 X)
    (ON X S)
    (ON S K)
    (ON K Z)
    (ON Z C)
    (ON C A)
    (ON J E)
    (ON E I)
    (ON I U)
    (ON U P)
    (HANDEMPTY)
)
(:goal (and
    (ON R G)
    (ON G D)
    (ON D W)
    (ON W T)
    (ON T C)
    (ON C I)
    (ON I F)
    (ON F A1)
    (ON A1 Y)
    (ON Y N)
    (ON N D1)
    (ON D1 E)
    (ON E L)
    (ON L C1)
    (ON C1 P)
    (ON P A)
    (ON A B1)
    (ON B1 S)
    (ON S M)
    (ON M V)
    (ON V O)
    (ON O Q)
    (ON Q Z)
    (ON Z H)
    (ON H B)
    (ON B J)
    (ON J X)
    (ON X U)
    (ON U K)
    (ON K E1)
)
)
)
