

(define (problem BW-rand-14)
(:domain blocksworld-4ops)
(:objects a b c d e f g h i j k l m n )
(:init
(handempty)
(on a j)
(on b g)
(on c f)
(on d a)
(on e m)
(ontable f)
(on g l)
(on h d)
(ontable i)
(on j b)
(on k c)
(ontable l)
(on m n)
(on n i)
(clear e)
(clear h)
(clear k)
)
(:goal
(and
(on a d)
(on b n)
(on c k)
(on e i)
(on g c)
(on h g)
(on i l)
(on j h)
(on l f)
(on m e))
)
)


