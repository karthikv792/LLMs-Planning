

(define (problem BW-rand-10)
(:domain blocksworld-4ops)
(:objects a b c d e f g h i j )
(:init
(handempty)
(on a e)
(ontable b)
(on c f)
(ontable d)
(ontable e)
(on f h)
(on g j)
(on h g)
(on i b)
(ontable j)
(clear a)
(clear c)
(clear d)
(clear i)
)
(:goal
(and
(on a c)
(on b f)
(on c g)
(on d i)
(on e h)
(on f e)
(on h j)
(on j a))
)
)


