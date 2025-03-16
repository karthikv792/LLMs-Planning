

(define (problem BW-rand-9)
(:domain blocksworld-4ops)
(:objects a b c d e f g h i )
(:init
(handempty)
(ontable a)
(on b c)
(ontable c)
(on d f)
(on e b)
(on f i)
(on g d)
(on h e)
(on i h)
(clear a)
(clear g)
)
(:goal
(and
(on c b)
(on d a)
(on e c)
(on f e)
(on g d)
(on h f))
)
)


