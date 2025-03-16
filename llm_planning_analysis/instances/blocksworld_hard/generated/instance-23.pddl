

(define (problem BW-rand-10)
(:domain blocksworld-4ops)
(:objects a b c d e f g h i j )
(:init
(handempty)
(on a d)
(on b g)
(on c a)
(ontable d)
(on e j)
(on f i)
(ontable g)
(on h c)
(on i b)
(ontable j)
(clear e)
(clear f)
(clear h)
)
(:goal
(and
(on a c)
(on b d)
(on c h)
(on d f)
(on f a)
(on g b)
(on h i)
(on i j))
)
)


