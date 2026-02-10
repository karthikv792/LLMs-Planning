(define (domain obfuscated_randomized_blocksworld)
    (:requirements :equality)
    (:predicates
        (tv30k33pzoulql6w ?x1 - object)
        (af9tse23ljclsqad ?x1 - object)
        (cql4o62p1yeke3ok )
        (a46zhwykn6jvbabk ?x1 - object)
        (b6e9q4r60gagvdcn ?x1 - object ?x2 - object)
    )
    
    (:action u64y1a9apusmslxb
     :parameters (?ob1 - object)
     :precondition (and (tv30k33pzoulql6w ?ob1) (af9tse23ljclsqad ?ob1) (cql4o62p1yeke3ok ))
     :effect (and
        (a46zhwykn6jvbabk ?ob1)
        (not (tv30k33pzoulql6w ?ob1))
        (not (af9tse23ljclsqad ?ob1))
        (not (cql4o62p1yeke3ok )))
    )


    (:action kip9uw781pv62umn
     :parameters (?ob1 - object)
     :precondition (a46zhwykn6jvbabk ?ob1)
     :effect (and
        (tv30k33pzoulql6w ?ob1)
        (cql4o62p1yeke3ok )
        (af9tse23ljclsqad ?ob1)
        (not (a46zhwykn6jvbabk ?ob1)))
    )


    (:action j4gv801gnu2it0yj
     :parameters (?ob1 - object ?ob2 - object)
     :precondition (and (tv30k33pzoulql6w ?ob2) (a46zhwykn6jvbabk ?ob1))
     :effect (and
        (cql4o62p1yeke3ok )
        (tv30k33pzoulql6w ?ob1)
        (b6e9q4r60gagvdcn ?ob1 ?ob2)
        (not (tv30k33pzoulql6w ?ob2))
        (not (a46zhwykn6jvbabk ?ob1)))
    )


    (:action wio5amhq7814n006
     :parameters (?ob1 - object ?ob2 - object)
     :precondition (and (b6e9q4r60gagvdcn ?ob1 ?ob2) (tv30k33pzoulql6w ?ob1) (cql4o62p1yeke3ok ))
     :effect (and
        (a46zhwykn6jvbabk ?ob1)
        (tv30k33pzoulql6w ?ob2)
        (not (b6e9q4r60gagvdcn ?ob1 ?ob2))
        (not (tv30k33pzoulql6w ?ob1))
        (not (cql4o62p1yeke3ok )))
    )

)