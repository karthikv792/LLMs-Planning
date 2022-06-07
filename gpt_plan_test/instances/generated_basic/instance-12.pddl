

(define (problem BW-rand-10)
(:domain blocksworld-4ops)
(:objects a b c d e f g h i j )
(:init
(handempty)
(on a j)
(on b h)
(on c b)
(on d g)
(ontable e)
(on f a)
(ontable g)
(ontable h)
(on i c)
(on j i)
(clear d)
(clear e)
(clear f)
)
(:goal
(and
(on d g)
(on e b)
(on h j)
(on i e)
(on j f))
)
)


