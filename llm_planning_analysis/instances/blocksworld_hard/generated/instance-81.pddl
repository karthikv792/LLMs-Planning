

(define (problem BW-rand-10)
(:domain blocksworld-4ops)
(:objects a b c d e f g h i j )
(:init
(handempty)
(on a j)
(on b c)
(on c h)
(on d g)
(on e a)
(ontable f)
(ontable g)
(on h i)
(on i f)
(on j d)
(clear b)
(clear e)
)
(:goal
(and
(on a j)
(on b a)
(on c e)
(on d c)
(on f h)
(on g f)
(on i d)
(on j g))
)
)


