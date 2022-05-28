(define (problem blocks-42-0)
(:domain blocks)
(:objects
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
    (CLEAR Z)
    (CLEAR O1)
    (CLEAR J)
    (CLEAR I1)
    (CLEAR Q)
    (CLEAR D1)
    (ONTABLE D)
    (ONTABLE U)
    (ONTABLE K)
    (ONTABLE T)
    (ONTABLE B)
    (ONTABLE P1)
    (ON Z E)
    (ON E C1)
    (ON C1 O)
    (ON O E1)
    (ON E1 D)
    (ON O1 I)
    (ON I N)
    (ON N A1)
    (ON A1 C)
    (ON C H)
    (ON H R)
    (ON R G1)
    (ON G1 H1)
    (ON H1 W)
    (ON W V)
    (ON V Y)
    (ON Y M)
    (ON M J1)
    (ON J1 B1)
    (ON B1 U)
    (ON J K)
    (ON I1 K1)
    (ON K1 S)
    (ON S F)
    (ON F T)
    (ON Q G)
    (ON G F1)
    (ON F1 P)
    (ON P M1)
    (ON M1 A)
    (ON A L1)
    (ON L1 X)
    (ON X L)
    (ON L B)
    (ON D1 N1)
    (ON N1 P1)
    (HANDEMPTY)
)
(:goal (and
    (ON O E)
    (ON E N1)
    (ON N1 U)
    (ON U J)
    (ON J L)
    (ON L Q)
    (ON Q K)
    (ON K O1)
    (ON O1 D1)
    (ON D1 I1)
    (ON I1 P1)
    (ON P1 M)
    (ON M F)
    (ON F I)
    (ON I T)
    (ON T S)
    (ON S H1)
    (ON H1 A1)
    (ON A1 B1)
    (ON B1 W)
    (ON W C)
    (ON C F1)
    (ON F1 B)
    (ON B D)
    (ON D H)
    (ON H P)
    (ON P L1)
    (ON L1 X)
    (ON X Y)
    (ON Y K1)
    (ON K1 Z)
    (ON Z V)
    (ON V M1)
    (ON M1 N)
    (ON N G)
    (ON G E1)
    (ON E1 C1)
    (ON C1 J1)
    (ON J1 G1)
    (ON G1 R)
    (ON R A)
)
)
)
