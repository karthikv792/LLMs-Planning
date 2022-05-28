(define (problem blocks-27-0)
(:domain blocks)
(:objects
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
    (CLEAR A)
    (CLEAR F)
    (CLEAR H)
    (CLEAR W)
    (CLEAR U)
    (CLEAR B)
    (ONTABLE A)
    (ONTABLE F)
    (ONTABLE M)
    (ONTABLE Z)
    (ONTABLE C)
    (ONTABLE T)
    (ON H V)
    (ON V A1)
    (ON A1 E)
    (ON E L)
    (ON L X)
    (ON X M)
    (ON W J)
    (ON J G)
    (ON G O)
    (ON O P)
    (ON P R)
    (ON R Z)
    (ON U D)
    (ON D Q)
    (ON Q S)
    (ON S I)
    (ON I K)
    (ON K N)
    (ON N C)
    (ON B Y)
    (ON Y T)
    (HANDEMPTY)
)
(:goal (and
    (ON Y F)
    (ON F L)
    (ON L W)
    (ON W V)
    (ON V E)
    (ON E M)
    (ON M Q)
    (ON Q G)
    (ON G K)
    (ON K S)
    (ON S T)
    (ON T H)
    (ON H R)
    (ON R X)
    (ON X C)
    (ON C P)
    (ON P Z)
    (ON Z A)
    (ON A A1)
    (ON A1 B)
    (ON B D)
    (ON D J)
    (ON J O)
    (ON O U)
    (ON U N)
    (ON N I)
)
)
)
