

(define (problem BW-rand-14)
(:domain blocksworld-4ops)
(:objects a b c d e f g h i j k l m n )
(:init
(handempty)
(on a d)
(on b m)
(on c g)
(on d h)
(on e c)
(ontable f)
(on g b)
(on h n)
(on i a)
(on j i)
(ontable k)
(on l k)
(on m j)
(ontable n)
(clear e)
(clear f)
(clear l)
)
(:goal
(and
(on a l)
(on b h)
(on e c)
(on f n)
(on g d)
(on i k)
(on k b)
(on m g))
)
)


