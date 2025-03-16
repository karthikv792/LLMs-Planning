

(define (problem BW-rand-11)
(:domain blocksworld-4ops)
(:objects a b c d e f g h i j k )
(:init
(handempty)
(on a e)
(on b g)
(on c d)
(on d j)
(on e i)
(on f b)
(ontable g)
(on h k)
(on i h)
(ontable j)
(on k f)
(clear a)
(clear c)
)
(:goal
(and
(on a f)
(on b c)
(on c e)
(on e j)
(on f b)
(on g i)
(on j g)
(on k h))
)
)


