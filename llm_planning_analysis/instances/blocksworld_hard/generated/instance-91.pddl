

(define (problem BW-rand-8)
(:domain blocksworld-4ops)
(:objects a b c d e f g h )
(:init
(handempty)
(ontable a)
(on b f)
(on c d)
(ontable d)
(on e g)
(on f a)
(on g c)
(on h e)
(clear b)
(clear h)
)
(:goal
(and
(on a b)
(on b f)
(on c d)
(on d e)
(on e a)
(on g c)
(on h g))
)
)


