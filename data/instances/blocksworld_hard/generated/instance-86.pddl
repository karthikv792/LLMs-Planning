

(define (problem BW-rand-13)
(:domain blocksworld-4ops)
(:objects a b c d e f g h i j k l m )
(:init
(handempty)
(on a h)
(on b e)
(on c i)
(ontable d)
(on e m)
(on f c)
(on g f)
(ontable h)
(on i a)
(on j b)
(on k j)
(on l g)
(on m d)
(clear k)
(clear l)
)
(:goal
(and
(on a d)
(on b l)
(on c b)
(on e k)
(on f j)
(on g f)
(on j a)
(on k c)
(on l i)
(on m h))
)
)


