(define (problem blocks-20-1)
(:domain blocks)
(:objects
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
    (CLEAR G)
    (ONTABLE N)
    (ONTABLE M)
    (ON A H)
    (ON H K)
    (ON K Q)
    (ON Q T)
    (ON T E)
    (ON E I)
    (ON I B)
    (ON B P)
    (ON P L)
    (ON L D)
    (ON D O)
    (ON O J)
    (ON J S)
    (ON S C)
    (ON C F)
    (ON F R)
    (ON R N)
    (ON G M)
    (HANDEMPTY)
)
(:goal (and
    (ON H N)
    (ON N A)
    (ON A Q)
    (ON Q I)
    (ON I O)
    (ON O S)
    (ON S D)
    (ON D C)
    (ON C K)
    (ON K P)
    (ON P L)
    (ON L J)
    (ON J T)
    (ON T G)
    (ON G M)
    (ON M F)
    (ON F B)
    (ON B R)
    (ON R E)
)
)
)
