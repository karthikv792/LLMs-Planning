

(define (problem BW-rand-10)
(:domain blocksworld-4ops)
(:objects a b c d e f g h i j )
(:init
(handempty)
(ontable a)
(on b a)
(on c i)
(on d j)
(on e g)
(on f c)
(on g b)
(ontable h)
(on i e)
(ontable j)
(clear d)
(clear f)
(clear h)
)
(:goal
(and
(on a i)
(on b g)
(on d h)
(on e d)
(on f b)
(on h j)
(on i f)
(on j c))
)
)


