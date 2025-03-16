

(define (problem BW-rand-12)
(:domain blocksworld-4ops)
(:objects a b c d e f g h i j k l )
(:init
(handempty)
(on a h)
(on b a)
(on c k)
(on d j)
(on e i)
(on f d)
(on g l)
(ontable h)
(ontable i)
(on j b)
(ontable k)
(on l c)
(clear e)
(clear f)
(clear g)
)
(:goal
(and
(on a j)
(on b d)
(on c g)
(on g l)
(on h e)
(on i b)
(on j f)
(on k i)
(on l h))
)
)


