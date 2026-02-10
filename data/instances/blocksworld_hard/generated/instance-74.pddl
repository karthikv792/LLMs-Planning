

(define (problem BW-rand-12)
(:domain blocksworld-4ops)
(:objects a b c d e f g h i j k l )
(:init
(handempty)
(on a i)
(on b f)
(on c e)
(on d k)
(on e j)
(on f c)
(ontable g)
(on h l)
(on i h)
(on j g)
(on k a)
(on l b)
(clear d)
)
(:goal
(and
(on b h)
(on c f)
(on d j)
(on e b)
(on f i)
(on i g)
(on j a)
(on k c)
(on l e))
)
)


