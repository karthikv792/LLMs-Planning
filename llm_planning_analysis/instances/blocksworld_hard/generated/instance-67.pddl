

(define (problem BW-rand-8)
(:domain blocksworld-4ops)
(:objects a b c d e f g h )
(:init
(handempty)
(on a c)
(on b h)
(ontable c)
(on d e)
(on e g)
(on f b)
(ontable g)
(on h d)
(clear a)
(clear f)
)
(:goal
(and
(on a b)
(on b h)
(on c d)
(on f e)
(on h g))
)
)


