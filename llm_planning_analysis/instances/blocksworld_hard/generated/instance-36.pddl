

(define (problem BW-rand-9)
(:domain blocksworld-4ops)
(:objects a b c d e f g h i )
(:init
(handempty)
(on a c)
(ontable b)
(on c i)
(on d g)
(ontable e)
(ontable f)
(on g a)
(on h d)
(on i b)
(clear e)
(clear f)
(clear h)
)
(:goal
(and
(on a d)
(on d e)
(on e b)
(on f g)
(on g a)
(on h i)
(on i c))
)
)


