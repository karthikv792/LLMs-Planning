

(define (problem BW-rand-11)
(:domain blocksworld-4ops)
(:objects a b c d e f g h i j k )
(:init
(handempty)
(ontable a)
(ontable b)
(ontable c)
(on d j)
(on e b)
(ontable f)
(on g h)
(on h k)
(on i d)
(on j c)
(on k a)
(clear e)
(clear f)
(clear g)
(clear i)
)
(:goal
(and
(on b g)
(on c f)
(on d b)
(on e d)
(on f j)
(on g a)
(on j h)
(on k c))
)
)


