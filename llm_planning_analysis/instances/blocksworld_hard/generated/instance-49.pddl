

(define (problem BW-rand-11)
(:domain blocksworld-4ops)
(:objects a b c d e f g h i j k )
(:init
(handempty)
(on a b)
(on b c)
(ontable c)
(ontable d)
(ontable e)
(on f g)
(on g h)
(on h d)
(on i f)
(on j k)
(on k i)
(clear a)
(clear e)
(clear j)
)
(:goal
(and
(on a c)
(on d g)
(on e a)
(on f e)
(on h d)
(on i b)
(on j h)
(on k f))
)
)


