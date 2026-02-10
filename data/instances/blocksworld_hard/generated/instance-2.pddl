

(define (problem BW-rand-10)
(:domain blocksworld-4ops)
(:objects a b c d e f g h i j )
(:init
(handempty)
(ontable a)
(on b e)
(on c j)
(ontable d)
(on e f)
(on f c)
(on g d)
(ontable h)
(ontable i)
(ontable j)
(clear a)
(clear b)
(clear g)
(clear h)
(clear i)
)
(:goal
(and
(on a i)
(on b f)
(on c j)
(on d a)
(on f h)
(on g e)
(on i b)
(on j d))
)
)


