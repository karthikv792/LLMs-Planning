

(define (problem BW-rand-11)
(:domain blocksworld-4ops)
(:objects a b c d e f g h i j k )
(:init
(handempty)
(on a i)
(on b c)
(ontable c)
(ontable d)
(on e d)
(on f b)
(ontable g)
(on h e)
(on i h)
(on j f)
(on k a)
(clear g)
(clear j)
(clear k)
)
(:goal
(and
(on a f)
(on c a)
(on d j)
(on e c)
(on g b)
(on h g)
(on i h)
(on j e)
(on k d))
)
)


