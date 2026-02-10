

(define (problem BW-rand-11)
(:domain blocksworld-4ops)
(:objects a b c d e f g h i j k )
(:init
(handempty)
(on a c)
(on b f)
(on c d)
(on d b)
(on e i)
(ontable f)
(on g k)
(on h g)
(on i h)
(on j a)
(ontable k)
(clear e)
(clear j)
)
(:goal
(and
(on a e)
(on b g)
(on d k)
(on f j)
(on g a)
(on h d)
(on i c)
(on k i))
)
)


