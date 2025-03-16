

(define (problem BW-rand-9)
(:domain blocksworld-4ops)
(:objects a b c d e f g h i )
(:init
(handempty)
(on a h)
(on b c)
(on c f)
(on d b)
(ontable e)
(on f e)
(on g i)
(on h d)
(ontable i)
(clear a)
(clear g)
)
(:goal
(and
(on a c)
(on b g)
(on c e)
(on d i)
(on e d)
(on h f))
)
)


