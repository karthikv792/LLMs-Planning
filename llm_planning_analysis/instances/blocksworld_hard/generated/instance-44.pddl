

(define (problem BW-rand-15)
(:domain blocksworld-4ops)
(:objects a b c d e f g h i j k l m n o )
(:init
(handempty)
(on a j)
(on b f)
(on c g)
(on d i)
(on e b)
(on f h)
(ontable g)
(on h c)
(ontable i)
(on j n)
(on k e)
(on l d)
(ontable m)
(on n k)
(on o l)
(clear a)
(clear m)
(clear o)
)
(:goal
(and
(on c k)
(on d b)
(on f o)
(on g a)
(on h l)
(on i d)
(on j h)
(on l i)
(on m c)
(on n g)
(on o j))
)
)


