

(define (problem BW-rand-13)
(:domain blocksworld-4ops)
(:objects a b c d e f g h i j k l m )
(:init
(handempty)
(on a c)
(ontable b)
(on c l)
(on d f)
(on e m)
(on f k)
(on g a)
(on h j)
(on i d)
(on j b)
(on k e)
(on l i)
(ontable m)
(clear g)
(clear h)
)
(:goal
(and
(on b e)
(on c l)
(on e f)
(on g b)
(on h a)
(on i h)
(on j c)
(on k m)
(on m g))
)
)


