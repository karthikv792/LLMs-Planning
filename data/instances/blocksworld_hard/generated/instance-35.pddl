

(define (problem BW-rand-8)
(:domain blocksworld-4ops)
(:objects a b c d e f g h )
(:init
(handempty)
(ontable a)
(on b c)
(on c g)
(on d a)
(on e b)
(on f d)
(on g f)
(ontable h)
(clear e)
(clear h)
)
(:goal
(and
(on a g)
(on b d)
(on e a)
(on f e)
(on g c)
(on h b))
)
)


