(define (problem blocks-50-1)
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
    (CLEAR J)
    (CLEAR D1)
    (CLEAR Y)
    (CLEAR U)
    (CLEAR Q)
    (ONTABLE L)
    (ONTABLE F1)
    (ONTABLE D)
    (ONTABLE U)
    (ONTABLE Q)
    (ON J E1)
    (ON E1 M)
    (ON M V)
    (ON V F)
    (ON F R)
    (ON R A1)
    (ON A1 M1)
    (ON M1 Z)
    (ON Z X)
    (ON X H1)
    (ON H1 K)
    (ON K E)
    (ON E N)
    (ON N O)
    (ON O S)
    (ON S B1)
    (ON B1 R1)
    (ON R1 Q1)
    (ON Q1 H)
    (ON H O1)
    (ON O1 T1)
    (ON T1 G)
    (ON G J1)
    (ON J1 P)
    (ON P K1)
    (ON K1 L)
    (ON D1 U1)
    (ON U1 B)
    (ON B S1)
    (ON S1 C1)
    (ON C1 X1)
    (ON X1 A)
    (ON A W)
    (ON W W1)
    (ON W1 I1)
    (ON I1 N1)
    (ON N1 V1)
    (ON V1 T)
    (ON T L1)
    (ON L1 F1)
    (ON Y P1)
    (ON P1 G1)
    (ON G1 C)
    (ON C I)
    (ON I D)
    (HANDEMPTY)
)
(:goal (and
    (ON C A)
    (ON A U1)
    (ON U1 N)
    (ON N M1)
    (ON M1 X1)
    (ON X1 K)
    (ON K H1)
    (ON H1 T1)
    (ON T1 V)
    (ON V E)
    (ON E Q)
    (ON Q R1)
    (ON R1 S)
    (ON S L1)
    (ON L1 I1)
    (ON I1 D1)
    (ON D1 W)
    (ON W M)
    (ON M R)
    (ON R G)
    (ON G S1)
    (ON S1 Y)
    (ON Y N1)
    (ON N1 B)
    (ON B H)
    (ON H E1)
    (ON E1 F1)
    (ON F1 P)
    (ON P F)
    (ON F D)
    (ON D C1)
    (ON C1 A1)
    (ON A1 O)
    (ON O U)
    (ON U L)
    (ON L B1)
    (ON B1 V1)
    (ON V1 W1)
    (ON W1 K1)
    (ON K1 I)
    (ON I O1)
    (ON O1 X)
    (ON X J)
    (ON J J1)
    (ON J1 T)
    (ON T G1)
    (ON G1 Z)
    (ON Z P1)
    (ON P1 Q1)
)
)
)
