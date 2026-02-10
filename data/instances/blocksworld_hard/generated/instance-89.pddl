

(define (problem BW-rand-8)
(:domain blocksworld-4ops)
(:objects a b c d e f g h )
(:init
(handempty)
(on a g)
(ontable b)
(on c b)
(on d e)
(on e f)
(on f h)
(ontable g)
(on h c)
(clear a)
(clear d)
)
(:goal
(and
(on a c)
(on b d)
(on d e)
(on e g)
(on g h)
(on h f))
)
)


