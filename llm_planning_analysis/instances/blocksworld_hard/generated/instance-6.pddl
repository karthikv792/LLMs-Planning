

(define (problem BW-rand-12)
(:domain blocksworld-4ops)
(:objects a b c d e f g h i j k l )
(:init
(handempty)
(on a b)
(ontable b)
(on c f)
(on d l)
(on e j)
(on f h)
(on g i)
(on h a)
(ontable i)
(ontable j)
(on k e)
(ontable l)
(clear c)
(clear d)
(clear g)
(clear k)
)
(:goal
(and
(on a b)
(on b e)
(on c l)
(on d i)
(on e j)
(on f g)
(on g h)
(on j f)
(on k a))
)
)


