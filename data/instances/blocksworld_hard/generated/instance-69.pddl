

(define (problem BW-rand-12)
(:domain blocksworld-4ops)
(:objects a b c d e f g h i j k l )
(:init
(handempty)
(on a e)
(on b d)
(on c a)
(on d k)
(on e f)
(on f j)
(on g h)
(ontable h)
(on i c)
(ontable j)
(on k g)
(on l b)
(clear i)
(clear l)
)
(:goal
(and
(on a l)
(on b d)
(on c i)
(on d g)
(on e f)
(on f j)
(on g c)
(on i h)
(on k e))
)
)


