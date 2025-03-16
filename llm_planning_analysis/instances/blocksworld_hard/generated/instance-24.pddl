

(define (problem BW-rand-10)
(:domain blocksworld-4ops)
(:objects a b c d e f g h i j )
(:init
(handempty)
(on a d)
(on b e)
(on c b)
(on d c)
(on e f)
(on f g)
(on g h)
(on h j)
(ontable i)
(on j i)
(clear a)
)
(:goal
(and
(on b a)
(on c e)
(on e i)
(on f b)
(on g d)
(on h f))
)
)


