

(define (problem BW-rand-10)
(:domain blocksworld-4ops)
(:objects a b c d e f g h i j )
(:init
(handempty)
(ontable a)
(on b g)
(on c d)
(ontable d)
(ontable e)
(ontable f)
(on g f)
(ontable h)
(on i c)
(on j h)
(clear a)
(clear b)
(clear e)
(clear i)
(clear j)
)
(:goal
(and
(on a h)
(on b c)
(on c d)
(on d a)
(on e i)
(on f j)
(on g b)
(on i f)
(on j g))
)
)


