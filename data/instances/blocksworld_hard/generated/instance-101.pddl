

(define (problem BW-rand-8)
(:domain blocksworld-4ops)
(:objects a b c d e f g h )
(:init
(handempty)
(on a d)
(ontable b)
(on c g)
(on d e)
(on e c)
(on f b)
(on g h)
(on h f)
(clear a)
)
(:goal
(and
(on b e)
(on e h)
(on f b)
(on g f)
(on h c))
)
)


