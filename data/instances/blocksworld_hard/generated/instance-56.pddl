

(define (problem BW-rand-10)
(:domain blocksworld-4ops)
(:objects a b c d e f g h i j )
(:init
(handempty)
(ontable a)
(on b e)
(on c h)
(on d g)
(on e f)
(on f c)
(on g a)
(on h d)
(on i b)
(ontable j)
(clear i)
(clear j)
)
(:goal
(and
(on a e)
(on b f)
(on d j)
(on g b))
)
)


