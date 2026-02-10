

(define (problem BW-rand-8)
(:domain blocksworld-4ops)
(:objects a b c d e f g h )
(:init
(handempty)
(on a f)
(ontable b)
(ontable c)
(ontable d)
(on e a)
(on f b)
(on g h)
(on h e)
(clear c)
(clear d)
(clear g)
)
(:goal
(and
(on a b)
(on b e)
(on c f)
(on d a)
(on e h)
(on f g)
(on h c))
)
)


