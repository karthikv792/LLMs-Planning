

(define (problem BW-rand-11)
(:domain blocksworld-4ops)
(:objects a b c d e f g h i j k )
(:init
(handempty)
(on a h)
(on b g)
(on c i)
(ontable d)
(on e k)
(on f a)
(ontable g)
(on h d)
(on i e)
(on j c)
(on k b)
(clear f)
(clear j)
)
(:goal
(and
(on c g)
(on d k)
(on e d)
(on f a)
(on g f)
(on h e)
(on j b)
(on k j))
)
)


