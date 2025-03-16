

(define (problem BW-rand-9)
(:domain blocksworld-4ops)
(:objects a b c d e f g h i )
(:init
(handempty)
(on a e)
(ontable b)
(on c f)
(on d h)
(on e b)
(on f g)
(on g d)
(on h i)
(on i a)
(clear c)
)
(:goal
(and
(on a f)
(on b h)
(on d e)
(on e g)
(on g b)
(on i d))
)
)


