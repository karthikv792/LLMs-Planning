

(define (problem BW-rand-9)
(:domain blocksworld-4ops)
(:objects a b c d e f g h i )
(:init
(handempty)
(ontable a)
(on b a)
(ontable c)
(on d b)
(on e i)
(on f g)
(on g e)
(on h c)
(on i h)
(clear d)
(clear f)
)
(:goal
(and
(on b f)
(on c h)
(on d c)
(on g a)
(on i d))
)
)


