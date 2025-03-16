

(define (problem BW-rand-10)
(:domain blocksworld-4ops)
(:objects a b c d e f g h i j )
(:init
(handempty)
(on a h)
(on b a)
(on c b)
(ontable d)
(on e i)
(on f c)
(ontable g)
(on h e)
(on i j)
(on j g)
(clear d)
(clear f)
)
(:goal
(and
(on b g)
(on c e)
(on e i)
(on f h)
(on h j)
(on i f))
)
)


