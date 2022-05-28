(define (problem blocks-50-0)
(:domain blocks)
(:objects
    X1
    W1
    V1
    U1
    T1
    S1
    R1
    Q1
    P1
    O1
    N1
    M1
    L1
    K1
    J1
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
    (CLEAR H)
    (CLEAR O)
    (CLEAR H1)
    (CLEAR S1)
    (CLEAR E)
    (CLEAR X)
    (ONTABLE P)
    (ONTABLE V)
    (ONTABLE I)
    (ONTABLE K1)
    (ONTABLE J1)
    (ONTABLE X)
    (ON H O1)
    (ON O1 U)
    (ON U P)
    (ON O C1)
    (ON C1 N1)
    (ON N1 W)
    (ON W L)
    (ON L T1)
    (ON T1 N)
    (ON N F1)
    (ON F1 T)
    (ON T Q1)
    (ON Q1 C)
    (ON C P1)
    (ON P1 B1)
    (ON B1 A1)
    (ON A1 A)
    (ON A R1)
    (ON R1 D1)
    (ON D1 V)
    (ON H1 M1)
    (ON M1 J)
    (ON J Q)
    (ON Q M)
    (ON M U1)
    (ON U1 Y)
    (ON Y E1)
    (ON E1 X1)
    (ON X1 G1)
    (ON G1 I)
    (ON S1 Z)
    (ON Z W1)
    (ON W1 I1)
    (ON I1 L1)
    (ON L1 K)
    (ON K F)
    (ON F R)
    (ON R V1)
    (ON V1 D)
    (ON D G)
    (ON G B)
    (ON B K1)
    (ON E S)
    (ON S J1)
    (HANDEMPTY)
)
(:goal (and
    (ON L1 Q)
    (ON Q G)
    (ON G K1)
    (ON K1 W1)
    (ON W1 T)
    (ON T A1)
    (ON A1 J1)
    (ON J1 C)
    (ON C E)
    (ON E S1)
    (ON S1 R1)
    (ON R1 Q1)
    (ON Q1 S)
    (ON S H)
    (ON H G1)
    (ON G1 M)
    (ON M N1)
    (ON N1 B)
    (ON B P1)
    (ON P1 N)
    (ON N L)
    (ON L B1)
    (ON B1 J)
    (ON J V1)
    (ON V1 T1)
    (ON T1 D1)
    (ON D1 I)
    (ON I H1)
    (ON H1 P)
    (ON P F1)
    (ON F1 U1)
    (ON U1 O)
    (ON O X)
    (ON X K)
    (ON K Y)
    (ON Y C1)
    (ON C1 M1)
    (ON M1 A)
    (ON A V)
    (ON V U)
    (ON U I1)
    (ON I1 R)
    (ON R O1)
    (ON O1 E1)
    (ON E1 D)
    (ON D W)
    (ON W F)
    (ON F X1)
    (ON X1 Z)
)
)
)
