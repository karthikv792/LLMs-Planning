

(define (problem BW-rand-13)
(:domain blocksworld-4ops)
(:objects a b c d e f g h i j k l m )
(:init
(handempty)
(on a k)
(ontable b)
(on c m)
(ontable d)
(on e c)
(on f d)
(ontable g)
(on h f)
(on i l)
(on j b)
(on k j)
(on l e)
(on m h)
(clear a)
(clear g)
(clear i)
)
(:goal
(and
(on d c)
(on e d)
(on f e)
(on g l)
(on h b)
(on j h)
(on k j)
(on l i)
(on m a))
)
)


