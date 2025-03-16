

(define (problem BW-rand-9)
(:domain blocksworld-4ops)
(:objects a b c d e f g h i )
(:init
(handempty)
(on a c)
(on b h)
(ontable c)
(on d e)
(on e a)
(on f i)
(on g b)
(ontable h)
(ontable i)
(clear d)
(clear f)
(clear g)
)
(:goal
(and
(on a h)
(on b a)
(on c d)
(on e f)
(on f c)
(on h i))
)
)


