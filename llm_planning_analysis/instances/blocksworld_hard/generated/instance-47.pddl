

(define (problem BW-rand-9)
(:domain blocksworld-4ops)
(:objects a b c d e f g h i )
(:init
(handempty)
(on a c)
(ontable b)
(ontable c)
(ontable d)
(on e i)
(on f g)
(on g h)
(on h d)
(on i b)
(clear a)
(clear e)
(clear f)
)
(:goal
(and
(on a b)
(on b g)
(on c a)
(on d i)
(on e d)
(on f c)
(on h e))
)
)


