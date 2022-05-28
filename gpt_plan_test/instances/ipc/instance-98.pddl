(define (problem blocks-48-1)
(:domain blocks)
(:objects
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
    (CLEAR Q)
    (CLEAR A)
    (CLEAR M1)
    (CLEAR M)
    (ONTABLE W)
    (ONTABLE H1)
    (ONTABLE V1)
    (ONTABLE M)
    (ON Q G1)
    (ON G1 R1)
    (ON R1 K1)
    (ON K1 R)
    (ON R S)
    (ON S F1)
    (ON F1 T)
    (ON T O1)
    (ON O1 Y)
    (ON Y C)
    (ON C H)
    (ON H T1)
    (ON T1 J)
    (ON J F)
    (ON F L1)
    (ON L1 K)
    (ON K D)
    (ON D U1)
    (ON U1 L)
    (ON L Z)
    (ON Z D1)
    (ON D1 O)
    (ON O Q1)
    (ON Q1 W)
    (ON A B)
    (ON B A1)
    (ON A1 J1)
    (ON J1 N1)
    (ON N1 S1)
    (ON S1 N)
    (ON N E)
    (ON E C1)
    (ON C1 U)
    (ON U P1)
    (ON P1 E1)
    (ON E1 I1)
    (ON I1 V)
    (ON V B1)
    (ON B1 X)
    (ON X H1)
    (ON M1 I)
    (ON I P)
    (ON P G)
    (ON G V1)
    (HANDEMPTY)
)
(:goal (and
    (ON W O)
    (ON O M)
    (ON M X)
    (ON X N1)
    (ON N1 S)
    (ON S L)
    (ON L I1)
    (ON I1 U)
    (ON U G)
    (ON G Q)
    (ON Q E1)
    (ON E1 O1)
    (ON O1 J)
    (ON J P)
    (ON P C1)
    (ON C1 B1)
    (ON B1 Q1)
    (ON Q1 C)
    (ON C V)
    (ON V I)
    (ON I B)
    (ON B L1)
    (ON L1 K)
    (ON K K1)
    (ON K1 T)
    (ON T A1)
    (ON A1 V1)
    (ON V1 F)
    (ON F D1)
    (ON D1 M1)
    (ON M1 N)
    (ON N A)
    (ON A G1)
    (ON G1 D)
    (ON D U1)
    (ON U1 R)
    (ON R J1)
    (ON J1 P1)
    (ON P1 T1)
    (ON T1 H1)
    (ON H1 F1)
    (ON F1 S1)
    (ON S1 E)
    (ON E R1)
    (ON R1 Y)
    (ON Y Z)
    (ON Z H)
)
)
)
