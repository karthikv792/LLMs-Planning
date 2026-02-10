

(define (problem BW-rand-9)
(:domain blocksworld-4ops)
(:objects a b c d e f g h i )
(:init
(handempty)
(ontable a)
(ontable b)
(on c b)
(on d a)
(on e h)
(ontable f)
(on g e)
(on h i)
(on i c)
(clear d)
(clear f)
(clear g)
)
(:goal
(and
(on a f)
(on b d)
(on c e)
(on d c)
(on e i)
(on f h)
(on h g)
(on i a))
)
)


