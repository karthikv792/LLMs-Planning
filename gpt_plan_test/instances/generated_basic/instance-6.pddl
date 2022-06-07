

(define (problem BW-rand-7)
(:domain blocksworld-4ops)
(:objects a b c d e f g )
(:init
(handempty)
(on a f)
(on b d)
(on c g)
(on d a)
(on e c)
(on f e)
(ontable g)
(clear b)
)
(:goal
(and
(on b a)
(on c e)
(on d b)
(on e d)
(on f c)
(on g f))
)
)


