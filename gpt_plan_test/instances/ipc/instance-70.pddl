(define (problem blocks-34-1)
(:domain blocks)
(:objects
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
    (CLEAR C)
    (CLEAR A)
    (CLEAR B1)
    (ONTABLE O)
    (ONTABLE R)
    (ONTABLE E1)
    (ON C F)
    (ON F A1)
    (ON A1 F1)
    (ON F1 Y)
    (ON Y W)
    (ON W L)
    (ON L Q)
    (ON Q E)
    (ON E M)
    (ON M I)
    (ON I B)
    (ON B U)
    (ON U X)
    (ON X N)
    (ON N G1)
    (ON G1 O)
    (ON A D)
    (ON D S)
    (ON S Z)
    (ON Z V)
    (ON V K)
    (ON K G)
    (ON G C1)
    (ON C1 T)
    (ON T D1)
    (ON D1 P)
    (ON P J)
    (ON J H1)
    (ON H1 H)
    (ON H R)
    (ON B1 E1)
    (HANDEMPTY)
)
(:goal (and
    (ON T Q)
    (ON Q I)
    (ON I H)
    (ON H N)
    (ON N Z)
    (ON Z A)
    (ON A D1)
    (ON D1 Y)
    (ON Y G)
    (ON G F)
    (ON F L)
    (ON L W)
    (ON W E1)
    (ON E1 J)
    (ON J C1)
    (ON C1 H1)
    (ON H1 U)
    (ON U V)
    (ON V B1)
    (ON B1 X)
    (ON X K)
    (ON K R)
    (ON R B)
    (ON B D)
    (ON D S)
    (ON S P)
    (ON P C)
    (ON C E)
    (ON E F1)
    (ON F1 M)
    (ON M O)
    (ON O G1)
    (ON G1 A1)
)
)
)
