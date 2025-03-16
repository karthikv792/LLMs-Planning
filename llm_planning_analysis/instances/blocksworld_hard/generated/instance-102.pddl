

(define (problem BW-rand-12)
(:domain blocksworld-4ops)
(:objects a b c d e f g h i j k l )
(:init
(handempty)
(on a d)
(on b g)
(on c j)
(ontable d)
(on e i)
(on f b)
(on g c)
(ontable h)
(on i l)
(ontable j)
(ontable k)
(on l k)
(clear a)
(clear e)
(clear f)
(clear h)
)
(:goal
(and
(on a b)
(on c a)
(on d f)
(on e l)
(on g d)
(on h e)
(on i c)
(on k j)
(on l g))
)
)


