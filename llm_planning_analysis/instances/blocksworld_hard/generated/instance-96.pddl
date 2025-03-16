

(define (problem BW-rand-12)
(:domain blocksworld-4ops)
(:objects a b c d e f g h i j k l )
(:init
(handempty)
(on a j)
(on b h)
(on c a)
(ontable d)
(on e c)
(on f e)
(on g l)
(on h d)
(ontable i)
(on j i)
(ontable k)
(on l f)
(clear b)
(clear g)
(clear k)
)
(:goal
(and
(on a e)
(on b g)
(on c i)
(on d k)
(on f c)
(on g f)
(on i d)
(on j a)
(on k j)
(on l b))
)
)


