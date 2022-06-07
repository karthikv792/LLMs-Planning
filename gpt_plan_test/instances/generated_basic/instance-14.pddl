

(define (problem BW-rand-11)
(:domain blocksworld-4ops)
(:objects a b c d e f g h i j k )
(:init
(handempty)
(on a b)
(on b g)
(on c k)
(ontable d)
(ontable e)
(on f e)
(ontable g)
(on h j)
(on i d)
(on j c)
(on k i)
(clear a)
(clear f)
(clear h)
)
(:goal
(and
(on b d)
(on c i)
(on d e)
(on f h)
(on h c)
(on i g)
(on j b)
(on k f))
)
)


