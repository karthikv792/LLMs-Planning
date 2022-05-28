(define (problem blocks-24-0)
(:domain blocks)
(:objects
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
    (CLEAR B)
    (CLEAR G)
    (CLEAR V)
    (CLEAR M)
    (CLEAR R)
    (ONTABLE T)
    (ONTABLE Q)
    (ONTABLE F)
    (ONTABLE H)
    (ONTABLE R)
    (ON B O)
    (ON O C)
    (ON C L)
    (ON L K)
    (ON K I)
    (ON I S)
    (ON S D)
    (ON D T)
    (ON G P)
    (ON P A)
    (ON A J)
    (ON J N)
    (ON N X)
    (ON X Q)
    (ON V W)
    (ON W E)
    (ON E U)
    (ON U F)
    (ON M H)
    (HANDEMPTY)
)
(:goal (and
    (ON I E)
    (ON E F)
    (ON F O)
    (ON O X)
    (ON X J)
    (ON J N)
    (ON N V)
    (ON V P)
    (ON P M)
    (ON M H)
    (ON H K)
    (ON K L)
    (ON L D)
    (ON D U)
    (ON U A)
    (ON A T)
    (ON T R)
    (ON R C)
    (ON C G)
    (ON G Q)
    (ON Q S)
    (ON S B)
    (ON B W)
)
)
)
