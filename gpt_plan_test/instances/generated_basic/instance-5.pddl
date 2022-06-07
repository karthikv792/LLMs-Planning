

(define (problem BW-rand-7)
(:domain blocksworld-4ops)
(:objects a b c d e f g )
(:init
(handempty)
(on a c)
(on b d)
(ontable c)
(ontable d)
(on e g)
(on f a)
(on g f)
(clear b)
(clear e)
)
(:goal
(and
(on b e)
(on c g)
(on d b)
(on f a)
(on g f))
)
)


