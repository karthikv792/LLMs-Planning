(define (problem BW-generalization-4)
(:domain mystery-4ops)(:objects f i j b k l h g c a e)
(:init 
(harmony)
(planet f)
(planet i)
(planet j)
(planet b)
(planet k)
(planet l)
(planet h)
(planet g)
(planet c)
(planet a)
(planet e)
(province f)
(province i)
(province j)
(province b)
(province k)
(province l)
(province h)
(province g)
(province c)
(province a)
(province e)
)
(:goal
(and
(craves f i)
(craves i j)
(craves j b)
(craves b k)
(craves k l)
(craves l h)
(craves h g)
(craves g c)
(craves c a)
(craves a e)
)))