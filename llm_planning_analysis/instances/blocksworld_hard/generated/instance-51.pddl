

(define (problem BW-rand-12)
(:domain blocksworld-4ops)
(:objects a b c d e f g h i j k l )
(:init
(handempty)
(on a b)
(on b h)
(on c d)
(on d l)
(on e c)
(on f a)
(on g f)
(on h i)
(on i k)
(ontable j)
(ontable k)
(ontable l)
(clear e)
(clear g)
(clear j)
)
(:goal
(and
(on a e)
(on c k)
(on d f)
(on e c)
(on f h)
(on g a)
(on h i)
(on j l)
(on l b))
)
)


