

(define (problem BW-rand-8)
(:domain blocksworld-4ops)
(:objects a b c d e f g h )
(:init
(handempty)
(ontable a)
(on b h)
(ontable c)
(on d e)
(on e a)
(on f g)
(on g b)
(on h d)
(clear c)
(clear f)
)
(:goal
(and
(on b f)
(on c d)
(on f g)
(on g a)
(on h b))
)
)


