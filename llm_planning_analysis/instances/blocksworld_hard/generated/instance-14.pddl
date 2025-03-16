

(define (problem BW-rand-9)
(:domain blocksworld-4ops)
(:objects a b c d e f g h i )
(:init
(handempty)
(ontable a)
(on b e)
(on c g)
(on d i)
(ontable e)
(on f h)
(on g d)
(on h a)
(on i f)
(clear b)
(clear c)
)
(:goal
(and
(on a d)
(on b f)
(on e a)
(on f g)
(on g h)
(on i b))
)
)


