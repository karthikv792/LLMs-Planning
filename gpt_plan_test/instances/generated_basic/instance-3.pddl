

(define (problem BW-rand-6)
(:domain blocksworld-4ops)
(:objects a b c d e f )
(:init
(handempty)
(ontable a)
(on b d)
(ontable c)
(on d e)
(on e c)
(ontable f)
(clear a)
(clear b)
(clear f)
)
(:goal
(and
(on a d)
(on b c)
(on c a)
(on e b))
)
)


