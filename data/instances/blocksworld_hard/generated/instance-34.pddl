

(define (problem BW-rand-7)
(:domain blocksworld-4ops)
(:objects a b c d e f g )
(:init
(handempty)
(on a g)
(ontable b)
(on c a)
(ontable d)
(on e d)
(on f e)
(on g f)
(clear b)
(clear c)
)
(:goal
(and
(on a f)
(on c d)
(on d a)
(on e b)
(on f e)
(on g c))
)
)


