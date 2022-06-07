

(define (problem BW-rand-11)
(:domain blocksworld-4ops)
(:objects a b c d e f g h i j k )
(:init
(handempty)
(ontable a)
(on b c)
(ontable c)
(ontable d)
(on e k)
(on f j)
(on g b)
(on h g)
(on i e)
(ontable j)
(on k h)
(clear a)
(clear d)
(clear f)
(clear i)
)
(:goal
(and
(on a i)
(on b a)
(on c j)
(on d e)
(on e b)
(on f g)
(on g c)
(on i f))
)
)


