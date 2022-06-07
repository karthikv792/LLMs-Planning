

(define (problem BW-rand-8)
(:domain blocksworld-4ops)
(:objects a b c d e f g h )
(:init
(handempty)
(on a g)
(ontable b)
(on c h)
(on d b)
(on e d)
(ontable f)
(on g c)
(on h f)
(clear a)
(clear e)
)
(:goal
(and
(on a g)
(on b h)
(on d f)
(on e b)
(on f c))
)
)


