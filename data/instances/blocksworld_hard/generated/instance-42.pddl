

(define (problem BW-rand-13)
(:domain blocksworld-4ops)
(:objects a b c d e f g h i j k l m )
(:init
(handempty)
(on a e)
(on b a)
(on c d)
(ontable d)
(on e j)
(on f l)
(on g m)
(on h g)
(ontable i)
(on j h)
(on k f)
(on l b)
(ontable m)
(clear c)
(clear i)
(clear k)
)
(:goal
(and
(on a b)
(on b j)
(on c d)
(on d g)
(on e c)
(on f l)
(on h i)
(on j h)
(on m k))
)
)


