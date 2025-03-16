

(define (problem BW-rand-9)
(:domain blocksworld-4ops)
(:objects a b c d e f g h i )
(:init
(handempty)
(on a b)
(on b h)
(on c a)
(on d f)
(on e i)
(on f e)
(ontable g)
(on h d)
(ontable i)
(clear c)
(clear g)
)
(:goal
(and
(on a d)
(on b a)
(on c b)
(on d f)
(on f i)
(on h e)
(on i h))
)
)


