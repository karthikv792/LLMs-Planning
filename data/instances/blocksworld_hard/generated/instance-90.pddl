

(define (problem BW-rand-9)
(:domain blocksworld-4ops)
(:objects a b c d e f g h i )
(:init
(handempty)
(ontable a)
(ontable b)
(on c b)
(on d f)
(on e d)
(ontable f)
(ontable g)
(on h e)
(ontable i)
(clear a)
(clear c)
(clear g)
(clear h)
(clear i)
)
(:goal
(and
(on a f)
(on b g)
(on c e)
(on e a)
(on f b)
(on h i)
(on i d))
)
)


