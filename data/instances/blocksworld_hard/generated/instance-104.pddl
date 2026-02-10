

(define (problem BW-rand-11)
(:domain blocksworld-4ops)
(:objects a b c d e f g h i j k )
(:init
(handempty)
(on a e)
(on b j)
(ontable c)
(on d i)
(on e g)
(on f b)
(on g c)
(on h a)
(on i k)
(ontable j)
(on k f)
(clear d)
(clear h)
)
(:goal
(and
(on a g)
(on b e)
(on c h)
(on d b)
(on f a)
(on h k)
(on i j)
(on j c))
)
)


