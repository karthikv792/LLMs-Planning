

(define (problem BW-rand-6)
(:domain blocksworld-4ops)
(:objects a b c d e f )
(:init
(handempty)
(on a e)
(on b f)
(on c a)
(ontable d)
(on e d)
(ontable f)
(clear b)
(clear c)
)
(:goal
(and
(on d c)
(on e a))
)
)


