

(define (problem BW-rand-10)
(:domain blocksworld-4ops)
(:objects a b c d e f g h i j )
(:init
(handempty)
(on a c)
(on b e)
(on c d)
(on d g)
(on e h)
(on f j)
(on g i)
(on h f)
(on i b)
(ontable j)
(clear a)
)
(:goal
(and
(on a b)
(on b f)
(on c h)
(on d a)
(on g c)
(on h d)
(on i g))
)
)


