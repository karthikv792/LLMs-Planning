

(define (problem BW-rand-8)
(:domain blocksworld-4ops)
(:objects a b c d e f g h )
(:init
(handempty)
(ontable a)
(ontable b)
(on c g)
(ontable d)
(on e h)
(on f b)
(on g a)
(on h f)
(clear c)
(clear d)
(clear e)
)
(:goal
(and
(on a e)
(on c g)
(on d a)
(on f c)
(on g h)
(on h b))
)
)


