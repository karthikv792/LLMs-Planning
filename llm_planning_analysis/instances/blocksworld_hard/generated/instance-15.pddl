

(define (problem BW-rand-9)
(:domain blocksworld-4ops)
(:objects a b c d e f g h i )
(:init
(handempty)
(on a g)
(on b c)
(ontable c)
(on d h)
(on e b)
(on f a)
(on g e)
(on h f)
(ontable i)
(clear d)
(clear i)
)
(:goal
(and
(on a i)
(on b a)
(on c e)
(on f h)
(on g d)
(on h g)
(on i c))
)
)


